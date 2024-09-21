#include <fstream>
#include <string>
#include <stdio.h>
#include <regex>
#include <queue>

int main()
{
    printf("Content-type: text/html\n\n");
    std::string uri(getenv("REQUEST_URI")), curlin;
    if(!std::regex_match(uri.substr(uri.size()-12, 12), std::regex(R"([0-9]{12})")))
    {
        printf("Illegal.\n");
        return 0;
    }
    std::string id = uri.substr(uri.size()-7, 7),
                probset = uri.substr(uri.size()-12, 5);
    std::ifstream probin(std::string("/usr/share/tfsys/") + probset);
    int siz;
    probin >> siz;
    getline(probin, curlin);
    printf(R"(<!DOCTYPE html>

<head>
    <title>[%s]作答: %s</title>
    <meta charset="UTF-8">
    <script>
)", probset.data(), id.data());
    std::queue<std::string> pbq, ansq;
    for(int i = 0; i < siz; i++)
    {
        getline(probin, curlin);
        if(curlin[0] == 'T') printf(R"(        right%d = %d;
        function true%d() { document.getElementById("curans%d").innerHTML = "(正确) "; right%d = 1;}
        function false%d() { document.getElementById("curans%d").innerHTML = "<span class='wa'>(错误) </span>"; right%d = 0;}
)", i,i,i,i,i,i,i,i);
        else printf(R"(        right%d = %d;
        function true%d() { document.getElementById("curans%d").innerHTML = "<span class='wa'>(正确) </span>"; right%d = 0;}
        function false%d() { document.getElementById("curans%d").innerHTML = "(错误) "; right%d = 1;}
)", i,i,i,i,i,i,i,i);
        pbq.push(curlin);
        getline(probin, curlin);
        ansq.push(curlin);
    }
    printf(R"(        function reqListener() {
            document.getElementById("remoteresult").innerHTML = this.responseText;
        }
        function on_submit() {
            let res = 0)");
    for(int i = 0; i < siz; i++) printf(" + right%d", i);
    printf(R"(;
            for(var ansspan of document.getElementsByClassName("ans")) {
                ansspan.style.display = "block";
            }
            for(var waspan of document.getElementsByClassName("wa")) {
                waspan.style.color = "#ff0000";
            }
            for(var btnspan of document.getElementsByClassName("solvbtn")) {
                btnspan.innerHTML = "";
            }
            document.getElementById("result").innerHTML = " 您总共答对了 " + res + " 道题。";
            const req = new XMLHttpRequest();
            req.addEventListener("load", reqListener);
            req.open("POST", "/cgi-bin/submitresult.cgi");
            req.send("%s %s "+res);
        }
    </script>
    <style>
        body{
            background-image: linear-gradient(90deg, rgb(252, 225, 185), rgb(255, 240, 217));
            text-align: center;
        }
        .AreaBox{
            text-align: left;
            background-color: #f0f0f0;
            border-style: solid;
            border-width: 1px;
            padding: 1cm 1cm 1cm 1cm;
            margin: auto;
            display: inline-block;
            box-shadow: 2px 2px 2px #909090;
        }
        #heada{
            font-size: 30px;
        }
        header{
            background-image: linear-gradient(90deg, #0070ff, #00b0f0);
            color: #ffffff;
            display: block;
        }
        button, input{
            padding: 2px 2px 2px 2px;
        }
        th, td {
            border:1px solid;
        }
    </style>
</head>

<body>
    <header>
        <h1>TFSys - 作答</h1>
    </header>
    <div class="AreaBox">
        <div id="heada">请判断对错并点击提交</div>
)", probset.data(), id.data());
    for(int i = 0; i < siz; i++)
    {
        curlin = pbq.front();
        pbq.pop();
        if(curlin[0] == 'T') printf(R"(        <span id="curans%d">() </span>
        <span class="solvbtn">
            <button onclick="true%d() ">正确</button>
            <button onclick="false%d() ">错误</button>
        </span>
        <span>%s</span>
        <br />
        <span class="ans" style="display: none;">正确答案: 正确<br />解析：%s</span><br />)",i,i,i,curlin.substr(1, curlin.size()-1).data(), ansq.front().data());
        else printf(R"(        <span id="curans%d">() </span>
        <span class="solvbtn">
            <button onclick="true%d() ">正确</button>
            <button onclick="false%d() ">错误</button>
        </span>
        <span>%s</span>
        <br />
        <span class="ans" style="display: none;">正确答案: 错误<br />解析：%s</span><br />)",i,i,i,curlin.substr(1, curlin.size()-1).data(), ansq.front().data());
        ansq.pop();
    }
    printf(R"(        <button onclick="on_submit() ">提交</button><span id="result"></span>
        <div id="remoteresult"></div>
    </div>
</body>)");
    return 0;
}