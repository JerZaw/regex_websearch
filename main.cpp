#include <iostream>
#include <regex>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>


std::string href_trim(std::string href_full){
    std::regex re(R"((https?):\/\/(www\.)?)");
    std::smatch m;
    std::regex_search(href_full,m,re);
    href_full=m.suffix();
    re = R"(.+?(?=\/))";
    std::regex_search(href_full,m,re);
    return m[0];
}

int main()
{
    std::regex relink(R"(<div class="g">.+?(?=<!--n-->))"),retel;
    std::smatch m;
    std::vector<std::string> strona_search;
    //std::vector<std::string> strona_link;
    //std::vector<std::string> strona_tel;
    std::map<std::string,std::vector<std::string>> strony;
    std::string pomlink,pom;
    std::vector<std::string> pomtel;

    std::fstream input_file("plik1.txt", std::ios::in);//WCZYTYWANIE+RODZIELANIE OSOBNYCH WYNIKÓW WYSZUKIWANIA
    if(input_file.is_open()){
        std::stringstream str_stream;
        str_stream << input_file.rdbuf();
        std::string full_text = str_stream.str();
        while(std::regex_search(full_text, m, relink)){
            strona_search.emplace_back(m[0]);
            full_text = m.suffix();
        }
    }
    input_file.close();
    input_file.open("plik2.txt", std::ios::in);
    if(input_file.is_open()){
        std::stringstream str_stream;
        str_stream << input_file.rdbuf();
        std::string full_text = str_stream.str();
        while(std::regex_search(full_text, m, relink)){
            strona_search.emplace_back(m[0]);
            full_text = m.suffix();
        }
    }
    input_file.close();
    input_file.open("plik3.txt", std::ios::in);
    if(input_file.is_open()){
        std::stringstream str_stream;
        str_stream << input_file.rdbuf();
        std::string full_text = str_stream.str();
        while(std::regex_search(full_text, m, relink)){
            strona_search.emplace_back(m[0]);
            full_text = m.suffix();
        }
    }
    input_file.close();
    input_file.open("plik4.txt", std::ios::in);
    if(input_file.is_open()){
        std::stringstream str_stream;
        str_stream << input_file.rdbuf();
        std::string full_text = str_stream.str();
        while(std::regex_search(full_text, m, relink)){
            strona_search.emplace_back(m[0]);
            full_text = m.suffix();
        }
    }
    input_file.close();
    relink = R"((<a href=").+?(?="))";
    retel = R"((((tel)|(Tel)).{0,8}\d{9})|((48)? ?\d\d((-)|( ))\d\d((-)|( ))\d\d((-)|( ))\d\d\d)|((48)? ?\d\d\d((-)|( ))\d\d((-)|( ))\d\d((-)|( ))\d\d)|((48)? ?\d\d((-)|( ))\d\d\d((-)|( ))\d\d((-)|( ))\d\d)|((48)? ?\d\d((-)|( ))\d\d((-)|( ))\d\d\d((-)|( ))\d\d)|(48\d{9})|(\(\d{2,3}\)[0-9\- ]{6,13}))";
    for(unsigned long long i=0;i<strona_search.size();i++){//ZNAJDYWANIE PRZYCIĘTYCH LINKÓW + NR TEL, DODAWANIE DO MAPY
        std::regex_search(strona_search[i], m, relink);
        pomlink = m[0];
        pomlink.erase(0,9);
        pomlink = href_trim(pomlink);
        //std::cout<<pomlink<<'\n';
        while(std::regex_search(strona_search[i], m, retel)){
            pomtel.emplace_back(m[0]);
            //std::cout<<m[0]<<'\n';
            strona_search[i]=m.suffix();
        }
        strony.emplace(pomlink,pomtel);
        pomtel.clear();
    }
    retel = R"((\d+))";//NORMALIZACJA NR TEL
    for(auto it=strony.begin();it!=strony.end();it++){
        for(unsigned long long i=0;i<it->second.size();i++){
            pom="";
            while(std::regex_search(it->second[i], m, retel)){
                pom += m[0];
                it->second[i]=m.suffix();
            }
            it->second[i]=pom;
        }
    }
    std::vector<std::string> uniquestr; //USUWANIE DUPLIKATÓW NR TEL
    for(auto it=strony.begin();it!=strony.end();it++){
        std::unique_copy(it->second.begin(),it->second.end(),std::back_inserter(uniquestr),[](std::string s1, std::string s2){return s1==s2;});
        it->second=uniquestr;
        uniquestr.clear();
    }

    std::cout<<"\n#########################################\n\n";

    for(auto it=strony.begin();it!=strony.end();it++){
        std::cout<<'\n'<<it->first<<'\n';
        for(unsigned long long i=0;i<it->second.size();i++){
            std::cout<<it->second[i]<<'\n';
        }
    }

    std::fstream output_file("linki_telefony.csv",std::ios::out);//ZAPIS DO CSV
    if(output_file.is_open()){
        for(auto &[link,telefony] : strony){
            output_file<<link;
            for(unsigned long long i=0;i<telefony.size();i++){
                output_file<<','<<telefony[i];
            }
            output_file<<'\n';
        }
    }
    output_file.close();

}
