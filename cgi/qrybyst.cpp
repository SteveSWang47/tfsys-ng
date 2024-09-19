#include <mysql++/mysql++.h>
#include <regex>
#include <iostream>
#include <stdio.h>

int main()
{
    std::string str;
    int pag;
    std::cin >> str >> pag;
    printf("Content-type: text/html\n\n");
    if(std::regex_match(str, std::regex(R"([0-9]{5})")))
    {
        mysqlpp::Connection conn("tfsys");
        mysqlpp::Query qry = conn.query("select from_unixtime(time), id, (select name from users where id = recs.id), (select class from users where id = recs.id), lpad(setid, 5, 0), (select name from sets where id = recs.setid), cast((result * 100 / (select full from sets where id = recs.setid)) as unsigned) from recs where setid="+str+" order by time desc offset " + std::to_string(pag*50) + " rows fetch first 50 rows only;");
        if(mysqlpp::StoreQueryResult res = qry.store())
        {
            std::cout << "<table>\n<thread><tr><th>时间</th><th>学工号</th><th>姓名</th><th>班级</th><th>题组编号</th><th>题组名称</th><th>得分</th></tr></thread>\n<tbody>";
            for(auto rw : res)
            {
                std::cout << "<tr>\n";
                for(auto cl : rw)
                {
                    std::cout << "<td>" << cl << "</td>\n";
                }
                std::cout << "</tr>\n";
            }
            std::cout << "</table><br />\n";
            if(pag > 0)
            {
                std::cout << "<a onclick=\"prvpagst()\">[上一页]</a>" << std::endl;
            }
            if(res.size() >= 50)
            {
                std::cout << "<a onclick=\"nxtpagst()\">[下一页]</a>" << std::endl;
            }
            printf("<br />\n");
        }
        else
        {
            std::cout << "ERROR: " << qry.error() << "请向弘毅27王思齐报告此问题。\n";
        }
    }
    else
    {
        printf("illegal.\n");
    }
    return 0;
}