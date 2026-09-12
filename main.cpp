#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

class Deveice
{

};

class DeviceManager
{
private:
    std::unordered_map<std::string, std::vector<Deveice*>> deviceMap;

public:
    const std::unordered_map<std::string, std::vector<Deveice*>>& getDeviceMap() const
    {
        return deviceMap;
    }
};

int main()
{
    std::vector<std::string> strings;
    strings.push_back("Hello");
    strings.push_back("World");

    // for (std::vector<std::string>::iterator it = strings.begin();
    //     it != strings.end(); ++it)
    // {
    //     std::cout << *it << std::endl;
    // }

    /* auto 适合长类型代码简化 若只是简单类型如int string等不要使用auto会影响代码可读性 */
    // for (auto it = strings.begin();
    //     it != strings.end(); ++it)
    // {
    //     std::cout << *it << std::endl;
    // }

    //两种写法
    using DeviceMap = std::unordered_map<std::string, std::vector<Deveice*>>;
    typedef std::unordered_map<std::string, std::vector<Deveice*>> DeviceMap;

    DeviceManager deviceManager;

    //三种写法

    //第一种写法
    // const std::unordered_map<std::string, std::vector<Deveice*>>& deviceMap = deviceManager.getDeviceMap();

    //第二种写法
    const DeviceMap& deviceMap = deviceManager.getDeviceMap();

    //第三种写法
    // const auto& deviceMap = deviceManager.getDeviceMap();
    

}