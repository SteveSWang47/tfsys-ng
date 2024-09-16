#include <mysql++/mysql++.h>
#include <regex>
#include <iostream>
#include <iomanip>
#include <format>
#include <stdio.h>
#include <time.h>

int main()
{
    std::string str;
    int pag;
    std::cin >> str >> pag;
    printf("Content-type: text/html\n\n");
    if(std::regex_match(str, std::regex(R"([0-9]{7})")))
    {
        mysqlpp::Connection conn("tfsys");
        mysqlpp::Query qry = conn.query("select * from recs where id="+str+" order by time desc offset " + std::to_string(pag*50) + " rows fetch first 50 rows only;");
        if(mysqlpp::StoreQueryResult res = qry.store())
        {
            std::cout << "<table>\n<thread><tr><th>学工号</th><th>题组</th><th>日期</th><th>时间</th><th>得分</th></tr></thread>\n<tbody>";
            time_t stp;
            struct tm *hora;
            for(auto rw : res)
            {
                std::cout << "<tr>\n";
                std::cout << "<td>" << rw[0] << "</td>\n";
                std::cout << "<td>" << std::setw(5) << std::setfill('0') << rw[1] << "</td>\n";
                stp = rw[2];
                hora = localtime(&stp);
                std::cout << std::format("<td>{}.{}.{}</td><td>{}:{}</td>\n", hora->tm_year+1900, hora->tm_mon+1, hora->tm_mday, hora->tm_hour, hora->tm_min);
                std::cout << "<td>" << rw[3] << "</td>\n";
                std::cout << "</tr>\n";
            }
            std::cout << "</table><br />\n";
            if(pag > 0)
            {
                std::cout << "<a onclick=\"prvpagid()\">[上一页]</a>" << std::endl;
            }
            if(res.size() >= 50)
            {
                std::cout << "<a onclick=\"nxtpagid()\">[下一页]</a>" << std::endl;
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