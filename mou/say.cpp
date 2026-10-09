#include "say.h"

int say(std::string word[] ){
   std::string path="";unsigned int slp=0;
   for (int i=0;i<20;i++) {
      if(word[i]!="") {
          path=path + "/home/root/mth/dt/sd/"+word[i]+".wav ";slp+=1200*1000;
      };
   }
   std::string aplay="aplay "+path +"&";

   std::cout<<std::endl<<aplay;

   const char* aplay_ = aplay.c_str();
   std::cout<<system(aplay_);
   usleep (slp);
   return 1;
}

int sayt(std::string word0,
        std::string word1="",
        std::string word2="",
        std::string word3="",
        std::string word4="",
        std::string word5="",
        std::string word6="",
        std::string word7="",
        std::string word8="",
        std::string word9="",
        std::string word10="",
        std::string word11="",
        std::string word12="",
        std::string word13="",
        std::string word14="",
        std::string word15="",
        std::string word16="",
        std::string word17="",
        std::string word18="",
        std::string word19=""
        ){
   std::string path="";unsigned int slp=0,t=1200*1000;

       if(word0!="") {path =path+"/home/root/mth/dt/sd/"+word0+".wav ";slp+=t;}
       if(word1!="") {path =path+"/home/root/mth/dt/sd/"+word1+".wav ";slp+=t;}
       if(word2!="") {path =path+"/home/root/mth/dt/sd/"+word2+".wav ";slp+=t;}
       if(word3!="") {path =path+"/home/root/mth/dt/sd/"+word3+".wav ";slp+=t;}
       if(word4!="") {path =path+"/home/root/mth/dt/sd/"+word4+".wav ";slp+=t;}
       if(word5!="") {path =path+"/home/root/mth/dt/sd/"+word5+".wav ";slp+=t;}
       if(word6!="") {path =path+"/home/root/mth/dt/sd/"+word6+".wav ";slp+=t;}
       if(word7!="") {path =path+"/home/root/mth/dt/sd/"+word7+".wav ";slp+=t;}
       if(word8!="") {path =path+"/home/root/mth/dt/sd/"+word8+".wav ";slp+=t;}
       if(word9!="") {path =path+"/home/root/mth/dt/sd/"+word9+".wav ";slp+=t;}
       if(word10!=""){path =path+"/home/root/mth/dt/sd/"+word10+".wav ";slp+=t;}
       if(word11!=""){path =path+"/home/root/mth/dt/sd/"+word11+".wav ";slp+=t;}
       if(word12!=""){path =path+"/home/root/mth/dt/sd/"+word12+".wav ";slp+=t;}
       if(word13!=""){path =path+"/home/root/mth/dt/sd/"+word13+".wav ";slp+=t;}
       if(word14!=""){path =path+"/home/root/mth/dt/sd/"+word14+".wav ";slp+=t;}
       if(word15!=""){path =path+"/home/root/mth/dt/sd/"+word15+".wav ";slp+=t;}
       if(word16!=""){path =path+"/home/root/mth/dt/sd/"+word16+".wav ";slp+=t;}
       if(word17!=""){path =path+"/home/root/mth/dt/sd/"+word17+".wav ";slp+=t;}
       if(word18!=""){path =path+"/home/root/mth/dt/sd/"+word18+".wav ";slp+=t;}
       if(word19!=""){path =path+"/home/root/mth/dt/sd/"+word19+".wav ";slp+=t;}


   std::string aplay="aplay "+path +"&";

   std::cout<<std::endl<<aplay;

   const char* aplay_ = aplay.c_str();
   std::cout<<system(aplay_);
   path="";
   usleep (slp);
   return 1;
}
