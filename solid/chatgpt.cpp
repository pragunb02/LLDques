#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

// Abstract base class for devices
class Device{
public:
    virtual std::string getName() const = 0;
    virtual ~Device() = default;
};

// Concrete Device classes
class Mobile : public Device {
public:
    std::string getName() const override {
        return "Mobile";
    }
};

class Laptop : public Device {
public:
    std::string getName() const override {
        return "Laptop";
    }
};

// Abstract base class for resolutions
class Resolution {
public:
    virtual std::string getQuality() const = 0;
    virtual int getPriority() const = 0; // Higher priority indicates better resolution
    virtual ~Resolution() = default;
};

// Concrete Resolution classes
class HD : public Resolution {
public:
    std::string getQuality() const override {
        return "720p";
    }

    int getPriority() const override {
        return 1;
    }
};

class FullHD : public Resolution {
public:
    std::string getQuality() const override {
        return "1080p";
    }

    int getPriority() const override {
        return 2;
    }
};

// Class to manage device-resolution combinations
class CombinationManager {
private:
    std::map<std::string, std::vector<std::shared_ptr<Resolution>>> deviceResolutions;

public:
    void addCombination(const std::shared_ptr<Device>& device, const std::shared_ptr<Resolution>& resolution) {
        deviceResolutions[device->getName()].push_back(resolution);
    }

    const std::vector<std::shared_ptr<Resolution>>& getResolutionsForDevice(const std::string& deviceName) const {
        static const std::vector<std::shared_ptr<Resolution>> emptyList;
        auto it = deviceResolutions.find(deviceName);
        return (it != deviceResolutions.end()) ? it->second : emptyList;
    }
};

// Class to handle video playback logic
class VideoPlayer {
private:
    CombinationManager& combinationManager;

public:
    VideoPlayer(CombinationManager& manager) : combinationManager(manager) {}

    void playVideo(const std::string& deviceName, const std::string& videoResolution) {
        const auto& resolutions = combinationManager.getResolutionsForDevice(deviceName);
        if (resolutions.empty()) {
            std::cout << "Device " << deviceName << " not found.\n";
            return;
        }

        auto match = std::find_if(resolutions.begin(), resolutions.end(),
            [&videoResolution](const std::shared_ptr<Resolution>& res) {
                return res->getQuality() == videoResolution;
            });

        if (match != resolutions.end()) {
            std::cout << "Playing video on " << deviceName << " at " << videoResolution << " resolution.\n";
        }
        else {
            auto bestResolution = std::max_element(resolutions.begin(), resolutions.end(),
                [](const std::shared_ptr<Resolution>& a, const std::shared_ptr<Resolution>& b) {
                    return a->getPriority() < b->getPriority();
                });

            std::cout << "Requested resolution not available. Playing video on " << deviceName
                << " at best available resolution: " << (*bestResolution)->getQuality() << ".\n";
        }
    }
};

int main() {
    // Create devices and resolutions
    std::shared_ptr<Device> mobile = std::make_shared<Mobile>();
    std::shared_ptr<Device> laptop = std::make_shared<Laptop>();

    std::shared_ptr<Resolution> hd = std::make_shared<HD>();
    std::shared_ptr<Resolution> fullHD = std::make_shared<FullHD>();

    // Manage combinations
    CombinationManager combinationManager;
    combinationManager.addCombination(mobile, hd);
    combinationManager.addCombination(mobile, fullHD);
    combinationManager.addCombination(laptop, fullHD);

    // Play videos
    VideoPlayer videoPlayer(combinationManager);

    videoPlayer.playVideo("Mobile", "720p"); // Exact match
    videoPlayer.playVideo("Laptop", "720p"); // Best available resolution
    videoPlayer.playVideo("Tablet", "1080p"); // Device not found

    return 0;
}
