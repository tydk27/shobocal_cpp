#include "shoboi_anime_list.hpp"

ShoboiClass::~ShoboiClass() {
    /*if (al != NULL) {
 *         delete[] al;
 *             }*/
}

int ShoboiClass::ShoboiMainProcess() {
    bool ret;
    ret = ShoboiRSSDownloader();
    if (!ret) {
        return 1;
    }

    struct ShoboiAnimeList *al;
    al = new ShoboiAnimeList[50];

    int num = 0;
    ret = ShoboiRSSAnalysis(al, num);
    if (!ret) {
        ShoboiRemove(al);
        return 1;
    }

    ShoboiPrint(al, num);

    ShoboiRemove(al);
    
    return 0;
}

bool ShoboiClass::ShoboiRSSDownloader() {
    try {
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(HOST, PORT);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator end;

        tcp::socket socket(io_service);
        boost::system::error_code error = boost::asio::error::host_not_found;
        while (error && endpoint_iterator != end) {
            socket.close();
            socket.connect(*endpoint_iterator++, error);
        }
        if (error) {
            throw boost::system::system_error(error);
        }

        boost::asio::streambuf request;
        ostream request_stream(&request);
        request_stream << "GET " << PATH << " HTTP/1.0\r\n";
        request_stream << "Host: " << HOST << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";

        boost::asio::write(socket, request);

        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");

        istream response_stream(&response);
        string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        string status_message;
        getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
            cout << "Invalid response\n";
            return false;
        }
        if (status_code != 200) {
            cout << "Response returned with status code " << status_code << "\n";
            return false;
        }

        boost::asio::read_until(socket, response, "\r\n\r\n");

        string header;
        while (getline(response_stream, header) && header != "\r") {
            /*cout << header << "\n";*/
        }
        /*cout << "\n";*/

        ofstream ofs(XML_PATH);

        if (response.size() > 0) {
            ofs << &response;
        }

        while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) {
            ofs << &response;
        }
        if (error != boost::asio::error::eof) {
            throw boost::system::system_error(error);
        }
        return true;
    }
    catch (std::exception& e) {
        cout << "Exception: " << e.what() << "\n";
        return false;
    }
}

bool ShoboiClass::ShoboiRSSAnalysis(struct ShoboiAnimeList *al, int &num) {
    try {
        ptree pt;
        read_xml(XML_PATH, pt);

        const ptree & format = pt.get_child("syobocal.ProgItems");
        BOOST_FOREACH(const ptree::value_type & child, format) {
            const ptree & attributes = child.second.get_child("<xmlattr>");

            BOOST_FOREACH(const ptree::value_type &value, attributes){
                const char *first_data = value.first.data();
                string second_data = value.second.data();

                ShoboiCompare(al, value, first_data, second_data, num);
            }
            num++;
        }
        return true;
    }
    catch (std::exception& e) {
        cout << "Exception: " << e.what() << "\n";
        return false;
    }
}

void ShoboiClass::ShoboiCompare(struct ShoboiAnimeList *al, const ptree::value_type &value, const char *first_data, string second_data, int &num) {
    if (strcmp(first_data, CMP_PID) == 0) {
        al[num].pid = second_data;
    }
    else if (strcmp(first_data, CMP_TID) == 0) {
        al[num].tid = second_data;
    }
    else if (strcmp(first_data, CMP_ST_TIME) == 0) {
        al[num].st_time = second_data;
    }
    else if (strcmp(first_data, CMP_ED_TIME) == 0) {
        al[num].ed_time = second_data;
    }
    else if (strcmp(first_data, CMP_CH_NAME) == 0) {
        al[num].ch_name = second_data;
    }
    else if (strcmp(first_data, CMP_CH_ID) == 0) {
        al[num].ch_id = second_data;
    }
    else if (strcmp(first_data, CMP_COUNT) == 0) {
        al[num].count = second_data;
    }
    else if (strcmp(first_data, CMP_ST_OFFSET) == 0) {
        al[num].st_offset = second_data;
    }
    else if (strcmp(first_data, CMP_SUB_TITLE) == 0) {
        al[num].sub_title = second_data;
    }
    else if (strcmp(first_data, CMP_TITLE) == 0) {
        al[num].title = second_data;
    }
    else if (strcmp(first_data, CMP_PROG_COMMENT) == 0) {
        al[num].prog_comment = second_data;
    }
}

void ShoboiClass::ShoboiPrint(struct ShoboiAnimeList *al, int &num) {
    string hhmm_date;
    for (int i = 0; i < num; i++) {
        hhmm_date = al[i].st_time;
        hhmm_date = hhmm_date.substr(8, 2) + ":" + hhmm_date.substr(10, 2);

        cout << hhmm_date << " [" << al[i].ch_name << "] " << al[i].title << " 第" << al[i].count << "話" << "『" << al[i].sub_title << "』\n";
    }
}

void ShoboiClass::ShoboiRemove(struct ShoboiAnimeList *al) {
    if (al != NULL) {
        delete[] al;
    }
    if (remove(XML_PATH) != 0) {
        cout << "Error deleting file\n";
    }
}
