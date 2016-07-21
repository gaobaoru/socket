

#include "Resource.h"

Resource::Resource(std::string loc, bool dir){
    location = loc;
    directory = dir;
    mimeType = "";
    size = 0;
    data = NULL;
}

Resource::~Resource() {
    if(data != NULL){
	delete data;
	data = NULL;	
    }
}







