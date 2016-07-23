

#include "ResourceHost.h"

ResourceHost::ResourceHost(std::string base){
    baseDiskPath = base;
}


ResourceHost::~ResourceHost(){
}


std::string ResourceHost::lookupMimeType(std::string ext){
    std::unordered_map<std::string, std::string>::const_iterator itor = mimeMap.find(ext);
    if(itor == mimeMap.end())
	return "text/html";

    return itor->second;
}


Resource* ResourceHost::readFile(std::string path, struct stat sb){

    if(!(sb.st_mode & S_IRWXU))
	return NULL;

    std::ifstream file;
    unsigned int len = 0;

    file.open(path.c_str(), std::ios::binary);

    if(!file.is_open())
	return NULL;

    len = sb.st_size;

    byte* fdata = new byte[len];
    file.read((char*)fdata, len);

    file.close();

    Resource* res = new Resource(path);
    res->setMimeType(lookupMimeType(res->getExtension()));
    res->setData(fdata, len);

    return res;
}


Resource* ResourceHost::readDirectory(std::string path, struct stat sb){
    Resource* res = NULL;
    if(path.empty() || path[path.length() - 1] != '/')
	path += '/';

    int numIndexs = sizeof(validIndexs) / sizeof(*validIndexs);
    std::string loadIndex;
    struct stat sidx;
    for(int i = 0; i < numIndexs; i++) {
	loadIndex = path + validIndexs[i];
     	if(stat(loadIndex.c_str(), &sidx) != -1)
	    return readFile(loadIndex.c_str(), sidx);
    }

    if(!(sb.st_mode & S_IRWXU))
	return NULL;

    std::string listing = generateDirList(path);
    unsigned int slen = listing.length();
    char* sdata = new char[slen];
    strncpy(sdata, listing.c_str(), slen);

    res = new Resource(path, true);
    res->setData((byte*)sdata, slen);

    return res;
}



std::string ResourceHost::generateDirList(std::string path) {

    size_t uri_pos = path.find_last_of(baseDiskPath);
    std::string uri = "?";
    if(uri_pos != std::string::npos)
	uri = path.substr(uri_pos);

    std::stringstream ret;
    ret << "<html><head><title>" << uri << "</title></head><body>";
    
    DIR *dir;
    struct dirent *ent;

    dir = opendir(path.c_str());
    if(dir == NULL)
	return "";

    ret << "<h1>Index of " << uri << "</h1><hr /><br />";

    while((ent = readdir(dir)) != NULL){
	if(ent->d_name[0] == '.')
	    continue;
  	ret << "<a href=\"" << uri << ent->d_name << "\">" << ent->d_name << "<\a><br />";
    }

    closedir(dir);

    ret << "</body></html>";

    return ret.str();
}


Resource* ResourceHost::getResource(std::string uri) {
    if(uri.length() > 255 || uri.empty())
	return NULL;

    std::string path = baseDiskPath + uri;
    Resource* res = NULL;

    struct stat sb;
    if(stat(path.c_str(), &sb) == -1)
	return NULL;

    if(sb.st_mode & S_IFDIR) {
	res = readDirectory(path, sb);
    }else if(sb.st_mode & S_IFREG){
	res = readFile(path, sb);
    }else {
	return NULL;
    }

    return res;
}








