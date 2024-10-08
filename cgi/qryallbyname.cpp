#include <mysql++/mysql++.h>
#include <iostream>
#include <stdio.h>

int main()
{
    printf("Content-type: text/html\n\n");
    mysqlpp::Connection conn("tfsys");
    mysqlpp::Query qry = conn.query("select from_unixtime(time), id, (select name from users where id = recs.id) nomen, (select class from users where id = recs.id), lpad(setid, 5, 0), (select name from sets where id = recs.setid), cast((result * 100 / (select full from sets where id = recs.setid)) as unsigned) from recs order by nomen, setid, time asc;");
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
    }
    else
    {
        std::cout << "ERROR: " << qry.error() << "请报告此问题。\n";
    }
    return 0;
}
