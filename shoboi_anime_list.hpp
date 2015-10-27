#ifndef SHOBOI_ANIME_LIST_H_
#define SHOBOI_ANIME_LIST_H_

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#define HOST "cal.syoboi.jp"
#define PORT "http"
#define PATH "./cal_chk.php?usr=tydk27&days=1"

#define XML_PATH "./cal_chk.php"

#define CMP_PID "PID"
#define CMP_TID "TID"
#define CMP_ST_TIME "StTime"
#define CMP_ED_TIME "EdTime"
#define CMP_CH_NAME "ChName"
#define CMP_CH_ID "ChID"
#define CMP_COUNT "Count"
#define CMP_ST_OFFSET "StOffset"
#define CMP_SUB_TITLE "SubTitle"
#define CMP_TITLE "Title"
#define CMP_PROG_COMMENT "ProgComment"

using namespace std;
using namespace boost;
using namespace boost::asio::ip;
using namespace boost::property_tree;

class ShoboiClass
{
public:
    ~ShoboiClass();
    int ShoboiMainProcess();
private:
    struct ShoboiAnimeList {
        string pid;
        string tid;
        string st_time;
        string ed_time;
        string ch_name;
        string ch_id;
        string count;
        string st_offset;
        string sub_title;
        string title;
        string prog_comment;
    };

    bool ShoboiRSSDownloader();
    bool ShoboiRSSAnalysis(struct ShoboiAnimeList *, int &);
    void ShoboiCompare(struct ShoboiAnimeList *, const ptree::value_type &, const char *, string, int &);
    void ShoboiPrint(struct ShoboiAnimeList *, int &);
    void ShoboiRemove(struct ShoboiAnimeList *);
};
#endif  /* SHOBOI_ANIME_LIST_H_ */
