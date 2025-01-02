#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

// Forward declarations
class IResolution;
class IDevice;
class IVideo;
class IVideoPlayer;

// Step 1: Interfaces
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
    virtual void addSupportedResolution(IResolution* res) = 0;
    virtual const std::vector<IResolution*>& getSupportedResolutions() const = 0;
    virtual ~IDevice() = default;
};

class IVideo {
public:
    virtual std::string getTitle() const = 0;
    virtual void addAvailableResolution(IResolution* res) = 0;
    virtual const std::vector<IResolution*>& getAvailableResolutions() const = 0;
    virtual ~IVideo() = default;
};

// Step 2: Resolution Strategy Interface
class IResolutionStrategy {
public:
    virtual IResolution* selectOptimalResolution(
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
    static IResolution* createHD() {
        return new Resolution(1280, 720, "HD");
    }

    static IResolution* createFullHD() {
        return new Resolution(1920, 1080, "Full HD");
    }

    static IResolution* create4K() {
        return new Resolution(3840, 2160, "4K");
    }
};

// Step 5: Device Class with Observer Pattern
class Device : public IDevice {
private:
    std::string name;
    int screenSize;
    std::vector<IResolution*> supportedResolutions;
    std::vector<void(*)(IResolution*)> observers;

public:
    Device(std::string n, int size) 
        : name(std::move(n)), screenSize(size) {}

    ~Device() {
        // Clean up supported resolutions
        for (auto res : supportedResolutions) {
            delete res;
        }
        supportedResolutions.clear();
    }

    std::string getName() const override { return name; }
    int getScreenSize() const override { return screenSize; }

    void addSupportedResolution(IResolution* res) override {
        supportedResolutions.push_back(res);
    }

    const std::vector<IResolution*>& getSupportedResolutions() const override {
        return supportedResolutions;
    }

    void addObserver(void(*observer)(IResolution*)) {
        observers.push_back(observer);
    }

    void notifyResolutionChange(IResolution* newRes) {
        for (auto observer : observers) {
            observer(newRes);
        }
    }
};

// Step 6: Video Class
class Video : public IVideo {
private:
    std::string title;
    std::vector<IResolution*> availableResolutions;

public:
    explicit Video(std::string t) : title(std::move(t)) {}

    ~Video() {
        // Clean up available resolutions
        for (auto res : availableResolutions) {
            delete res;
        }
        availableResolutions.clear();
    }

    std::string getTitle() const override { return title; }

    void addAvailableResolution(IResolution* res) override {
        availableResolutions.push_back(res);
    }

    const std::vector<IResolution*>& getAvailableResolutions() const override {
        return availableResolutions;
    }
};

// Step 7: Resolution Strategy
class OptimalResolutionStrategy : public IResolutionStrategy {
public:
    IResolution* selectOptimalResolution(
        const IDevice& device, 
        const IVideo& video
    ) override {
        const auto& deviceResolutions = device.getSupportedResolutions();
        const auto& videoResolutions = video.getAvailableResolutions();

        IResolution* optimalRes = nullptr;
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

        return optimalRes;  // Note: This returns a pointer to an existing resolution
    }
};

// Step 8: Video Player
class VideoPlayer {
private:
    IResolutionStrategy* resolutionStrategy;

public:
    explicit VideoPlayer(IResolutionStrategy* strategy)
        : resolutionStrategy(strategy) {}

    ~VideoPlayer() {
        delete resolutionStrategy;
    }

    void playVideo(const IVideo& video, IDevice& device) {
        try {
            IResolution* optimalResolution = resolutionStrategy->selectOptimalResolution(device, video);
            device.notifyResolutionChange(optimalResolution);
            
            std::cout << "Playing " << video.getTitle() << " at ";
            optimalResolution->displayInfo();
        }
        catch (const std::exception& e) {
            std::cerr << "Error playing video: " << e.what() << std::endl;
        }
    }
};

// Example Usage
void resolutionChangeCallback(IResolution* newRes) {
    std::cout << "Resolution changed to: ";
    newRes->displayInfo();
}

int main() {
    try {
        // Create device
        Device* phone = new Device("Smartphone", 6);
        phone->addSupportedResolution(ResolutionFactory::createHD());
        phone->addSupportedResolution(ResolutionFactory::createFullHD());
        phone->addObserver(resolutionChangeCallback);

        // Create video
        Video* movie = new Video("Sample Movie");
        movie->addAvailableResolution(ResolutionFactory::createHD());
        movie->addAvailableResolution(ResolutionFactory::createFullHD());
        movie->addAvailableResolution(ResolutionFactory::create4K());

        // Create strategy and player
        OptimalResolutionStrategy* strategy = new OptimalResolutionStrategy();
        VideoPlayer* player = new VideoPlayer(strategy);

        // Play video
        player->playVideo(*movie, *phone);

        // Cleanup
        delete player;  // This will also delete the strategy
        delete movie;   // This will delete its resolutions
        delete phone;   // This will delete its resolutions

    }
    catch (const std::exception& e) {
        std::cerr << "Error in main: " << e.what() << std::endl;
    }

    return 0;
}