#include <mysql++/mysql++.h>
#include <iostream>
#include <stdio.h>

int main()
{
    printf("Content-type: text/csv\n\n");
    mysqlpp::Connection conn("tfsys");
    mysqlpp::Query qry = conn.query("select from_unixtime(time), id, (select name from users where id = recs.id), (select class from users where id = recs.id), lpad(setid, 5, 0), (select name from sets where id = recs.setid), cast((result * 100 / (select full from sets where id = recs.setid)) as unsigned) from recs order by time desc;");
    if(mysqlpp::StoreQueryResult res = qry.store())
    {
        int i;
        for(auto rw : res)
        {
            i = 1;
            for(auto cl : rw)
            {
                std::cout << cl;
                if(i < rw.size()) std::cout << ", ";
                ++i;
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }
    else
    {
        std::cout << "ERROR: " << qry.error() << "请报告此问题。\n";
    }
    return 0;
}
