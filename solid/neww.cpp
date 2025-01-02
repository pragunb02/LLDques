#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>

// Forward declarations
class IResolution;
class IDevice;
class IVideo;
class IVideoPlayer;

// Step 1: Define interfaces
class IResolution {
public:
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual std::string getQuality() const = 0;
    virtual void displayInfo() const = 0;
    virtual ~IResolution() = default;
};

class IDevice {
public:
    virtual std::string getName() const = 0;
    virtual int getScreenSize() const = 0;
    virtual void addSupportedResolution(std::shared_ptr<IResolution> res) = 0;
    virtual const std::vector<std::shared_ptr<IResolution>>& getSupportedResolutions() const = 0;
    virtual ~IDevice() = default;
};

class IVideo {
public:
    virtual std::string getTitle() const = 0;
    virtual void addAvailableResolution(std::shared_ptr<IResolution> res) = 0;
    virtual const std::vector<std::shared_ptr<IResolution>>& getAvailableResolutions() const = 0;
    virtual ~IVideo() = default;
};

// Step 2: Resolution Strategy Interface
class IResolutionStrategy {
public:
    virtual std::shared_ptr<IResolution> selectOptimalResolution(
        const IDevice& device, 
        const IVideo& video
    ) = 0;
    virtual ~IResolutionStrategy() = default;
};

// Step 3: Concrete Resolution Class
class Resolution : public IResolution {
private:
    int width;
    int height;
    std::string quality;

public:
    Resolution(int w, int h, std::string q) 
        : width(w), height(h), quality(std::move(q)) {}

    int getWidth() const override { return width; }
    int getHeight() const override { return height; }
    std::string getQuality() const override { return quality; }
    
    void displayInfo() const override {
        std::cout << "Resolution: " << width << "x" << height 
                 << " (" << quality << ")" << std::endl;
    }
};

// Step 4: Resolution Factory
class ResolutionFactory {
public:
    static std::shared_ptr<IResolution> createHD() {
        return std::make_shared<Resolution>(1280, 720, "HD");
    }

    static std::shared_ptr<IResolution> createFullHD() {
        return std::make_shared<Resolution>(1920, 1080, "Full HD");
    }

    static std::shared_ptr<IResolution> create4K() {
        return std::make_shared<Resolution>(3840, 2160, "4K");
    }
};

// Step 5: Device Class with Observer Pattern
class Device : public IDevice {
private:
    std::string name;
    int screenSize;
    std::vector<std::shared_ptr<IResolution>> supportedResolutions;
    std::vector<std::function<void(std::shared_ptr<IResolution>)>> observers;

public:
    Device(std::string n, int size) 
        : name(std::move(n)), screenSize(size) {}

    std::string getName() const override { return name; }
    int getScreenSize() const override { return screenSize; }

    void addSupportedResolution(std::shared_ptr<IResolution> res) override {
        supportedResolutions.push_back(res);
    }

    const std::vector<std::shared_ptr<IResolution>>& getSupportedResolutions() const override {
        return supportedResolutions;
    }

    void addObserver(std::function<void(std::shared_ptr<IResolution>)> observer) {
        observers.push_back(std::move(observer));
    }

    void notifyResolutionChange(std::shared_ptr<IResolution> newRes) {
        for (const auto& observer : observers) {
            observer(newRes);
        }
    }
};

// Step 6: Concrete Video Class
class Video : public IVideo {
private:
    std::string title;
    std::vector<std::shared_ptr<IResolution>> availableResolutions;

public:
    explicit Video(std::string t) : title(std::move(t)) {}

    std::string getTitle() const override { return title; }

    void addAvailableResolution(std::shared_ptr<IResolution> res) override {
        availableResolutions.push_back(res);
    }

    const std::vector<std::shared_ptr<IResolution>>& getAvailableResolutions() const override {
        return availableResolutions;
    }
};

// Step 7: Concrete Resolution Strategy
class OptimalResolutionStrategy : public IResolutionStrategy {
public:
    std::shared_ptr<IResolution> selectOptimalResolution(
        const IDevice& device, 
        const IVideo& video
    ) override {
        const auto& deviceResolutions = device.getSupportedResolutions();
        const auto& videoResolutions = video.getAvailableResolutions();

        // Find highest common resolution
        std::shared_ptr<IResolution> optimalRes = nullptr;
        int maxWidth = 0;

        for (const auto& videoRes : videoResolutions) {
            for (const auto& deviceRes : deviceResolutions) {
                if (videoRes->getWidth() == deviceRes->getWidth() && 
                    videoRes->getHeight() == deviceRes->getHeight()) {
                    if (videoRes->getWidth() > maxWidth) {
                        maxWidth = videoRes->getWidth();
                        optimalRes = videoRes;
                    }
                }
            }
        }

        if (!optimalRes) {
            throw std::runtime_error("No compatible resolution found");
        }

        return optimalRes;
    }
};

// Step 8: Video Player Class
class VideoPlayer {
private:
    std::shared_ptr<IResolutionStrategy> resolutionStrategy;

public:
    explicit VideoPlayer(std::shared_ptr<IResolutionStrategy> strategy)
        : resolutionStrategy(std::move(strategy)) {}

    void playVideo(const IVideo& video, IDevice& device) {
        try {
            auto optimalResolution = resolutionStrategy->selectOptimalResolution(device, video);
            device.notifyResolutionChange(optimalResolution);
            
            std::cout << "Playing " << video.getTitle() << " at ";
            optimalResolution->displayInfo();
        }
        catch (const std::exception& e) {
            std::cerr << "Error playing video: " << e.what() << std::endl;
        }
    }
};

// Example usage
int main() {
    try {
        // Create device
        auto phone = std::make_shared<Device>("Smartphone", 6);
        phone->addSupportedResolution(ResolutionFactory::createHD());
        phone->addSupportedResolution(ResolutionFactory::createFullHD());

        // Add resolution change observer
        phone->addObserver([](std::shared_ptr<IResolution> newRes) {
            std::cout << "Resolution changed to: ";
            newRes->displayInfo();
        });

        // Create video
        auto movie = std::make_shared<Video>("Sample Movie");
        movie->addAvailableResolution(ResolutionFactory::createHD());
        movie->addAvailableResolution(ResolutionFactory::createFullHD());
        movie->addAvailableResolution(ResolutionFactory::create4K());

        // Create video player with strategy
        auto player = VideoPlayer(std::make_shared<OptimalResolutionStrategy>());
        
        // Play video
        player.playVideo(*movie, *phone);
    }
    catch (const std::exception& e) {
        std::cerr << "Error in main: " << e.what() << std::endl;
    }

    return 0;
}