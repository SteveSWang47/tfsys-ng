#include <mysql++/mysql++.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <iomanip>
#include <string.h>
#include <time.h>

int main()
{
    std::string id, setid;
    int res, cres;
    std::cin >> setid >> id >> cres;
    if(!std::regex_match(id, std::regex("[0-9]{7}")) || !std::regex_match(setid, std::regex("[0-9]{5}")))
    {
        printf("Content-type: text/html\n\nIllegal.\n");
        return 0;
    }
    mysqlpp::Connection conn("tfsys");
    conn.set_option(new mysqlpp::MultiStatementsOption(true));
    mysqlpp::Query qry(&conn, true); 
    qry << "select result from recs where id=" << id << " and setid=" << setid << ';';
    if(qry.store().size() > 50)
    {
        printf("Content-type: text/html\n\n本项目您的提交次数过多，已无法继续提交。");
        return 0;
    }
    qry = mysqlpp::Query(&conn, true);
    qry << "insert into recs values (" << id << ", " << setid << ", " << time(0) << ", " << cres << "); select time, result from recs where id=" << id << " and setid=" << setid << " order by time desc;";
    printf("Content-type: text/html\n\n");
    qry.store();
    if(mysqlpp::StoreQueryResult res = qry.store_next())
    {
        printf("本题您累计总共挑战过 %d 次。<br />\n", res.size());
        std::cout << "<table>\n<thread><tr><th>日期</th><th>时间</th><th>得分</th></tr></thread>\n<tbody>";
        time_t stp;
        struct tm *hora;
        for(auto rw : res)
        {
            std::cout << "<tr>\n";
            stp = rw[0];
            hora = localtime(&stp);
            std::cout << "<td>" << hora->tm_year+1900 << "." << hora->tm_mon+1 << "." << hora->tm_mday << "</td><td>" << std::setw(2) << std::setfill('0') << hora->tm_hour << ":" << std::setw(2) << std::setfill('0') << hora->tm_min <<"</td>\n"
                      << "<td>" << rw[1] << "</td>\n"
                      << "</tr>\n";
        }
        std::cout << "</table><br />\n";
    }
    return 0;
}