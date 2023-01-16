/*************************************************************************
    > File Name: XmlConfig.cpp
    > Author: hsz
    > Brief:
    > Created Time: Thu 12 Jan 2023 10:18:34 AM CST
 ************************************************************************/

#include "XmlConfig.h"
#include "rwmutex.h"
#include "tinyxml2.h"

static inline RWMutex *toMutex(void *mtx)
{
    return static_cast<RWMutex *>(mtx);
}

bool trim(std::string &str, const char *c = " \t\r\n")
{
    size_t begin = str.find_first_not_of(c);
    if (begin == std::string::npos) {
        return false;
    }

    size_t end = str.find_last_not_of(c);
    if (end == std::string::npos) {
        return false;
    }

    std::string temp(str.c_str() + begin, end - begin + 1);
    std::swap(temp, str);
    return true;
}

namespace eular {
XmlConfig::XmlConfig() :
    m_mutex(new RWMutex())
{

}

XmlConfig::~XmlConfig()
{
    delete m_mutex;
}

bool XmlConfig::loadFile(const std::string &xmlFile)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(xmlFile.c_str());
    if (doc.ErrorID()) {
        return false;
    }

    toMutex(m_mutex)->wlock();

    tinyxml2::XMLElement *curr = doc.RootElement();
    for (; curr; curr = curr->NextSiblingElement()) {
        loadXml(curr->Name(), curr);
    }

    toMutex(m_mutex)->wunlock();
    return true;
}

bool XmlConfig::parse(const std::string &xmlText)
{
    tinyxml2::XMLDocument doc;
    doc.Parse(xmlText.c_str());
    if (doc.ErrorID()) {
        return false;
    }

    toMutex(m_mutex)->wlock();

    tinyxml2::XMLElement *curr = doc.RootElement();
    for (; curr; curr = curr->NextSiblingElement()) {
        loadXml(curr->Name(), curr);
    }

    toMutex(m_mutex)->wunlock();
    return true;
}

void XmlConfig::foreach()
{
    toMutex(m_mutex)->rlock();

    for (auto it = m_xmlMap.begin(); it != m_xmlMap.end(); ++it) {
        printf("[%s, '%s']\n", it->first.c_str(), it->second.c_str());
    }

    toMutex(m_mutex)->runlock();
}

void XmlConfig::loadXml(std::string prefix, void *e)
{
    tinyxml2::XMLElement *ele = static_cast<tinyxml2::XMLElement *>(e);
    if (!ele) {
        return;
    }

    tinyxml2::XMLElement *curr = ele->FirstChildElement();
    for (; curr; curr = curr->NextSiblingElement()) {
        std::string key = prefix.empty() ? curr->Name() : prefix + "." + curr->Name();
        std::string value = curr->GetText() ? curr->GetText() : "";
        trim(value);
        m_xmlMap.insert(std::make_pair(key, value));
        if (curr->NoChildren() == false) {
            loadXml(key, curr);
        }
    }
}

} // namespace eular
