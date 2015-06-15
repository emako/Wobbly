#ifndef WOBBLYPROJECT_H
#define WOBBLYPROJECT_H


#include <cstdint>

#include <unordered_map>
#include <map>

#include <set>

#include <array>
#include <vector>
#include <string>

#include "WobblyException.h"


/*
static const char[] match_chars = { 'p', 'c', 'n', 'b', 'u' };


enum Matches {
    P = 0,
    C,
    N,
    B,
    U
};
*/


static inline uint8_t matchCharToIndex(char match) {
    if (match == 'p')
        return 0;
    if (match == 'c')
        return 1;
    if (match == 'n')
        return 2;
    if (match == 'b')
        return 3;
    if (match == 'u')
        return 4;

    return 255;
}


struct FreezeFrame {
    int first;
    int last;
    int replacement;
};


struct Preset {
    std::string name; // Must be suitable for use as Python function name.
    std::string contents;
};


struct Section {
    int start;
    std::vector<std::string> presets; // Preset names, in user-defined order.
    int64_t fps_num;
    int64_t fps_den;
    int num_frames; // If the presets don't change the frame count, this is the same as the original number of frames. Or -1?

    Section(int _start, int64_t _fps_num = 0, int64_t _fps_den = 0, int _num_frames = 0)
        : start(_start)
        , fps_num(_fps_num)
        , fps_den(_fps_den)
        , num_frames(_num_frames)
    { }
};


struct FrameRange {
    int first;
    int last;
};


struct CustomList {
    std::string name;
    std::string preset; // Preset name.
    std::map<int, FrameRange> frames; // Key is FrameRange::first

    void addFrameRange(int first, int last) {
        const FrameRange *overlap = findFrameRange(first);
        if (!overlap)
            overlap = findFrameRange(last);
        if (!overlap) {
            auto it = frames.upper_bound(first);
            if (it != frames.cend() && it->second.first < last)
                overlap = &it->second;
        }

        if (overlap)
            throw WobblyException("Can't add range (" + std::to_string(first) + "," + std::to_string(last) + ") to custom list '" + name + "': overlaps (" + std::to_string(overlap->first) + "," + std::to_string(overlap->last) + ").");

        FrameRange range = { first, last };
        frames.insert(std::make_pair(range.first, range));
    }

    void deleteFrameRange(int first) {
        frames.erase(first);
    }

    const FrameRange *findFrameRange(int frame) const {
        if (!frames.size())
            return nullptr;

        auto it = frames.upper_bound(frame);

        it--;

        if (frame <= it->second.last)
            return &it->second;

        return nullptr;
    }
};


struct Resize {
    int width;
    int height;
};


struct Crop {
    int left;
    int top;
    int right;
    int bottom;
};


enum PositionInFilterChain {
    PostSource = 0,
    PostFieldMatch,
    PostDecimate
};


class WobblyProject {
    public:
        std::string project_path;
        int num_frames[3];

        int64_t fps_num;
        int64_t fps_den;

        std::string input_file;
        std::map<int, FrameRange> trims; // Key is FrameRange::first
        std::unordered_map<std::string, double> vfm_parameters;
        std::unordered_map<std::string, double> vdecimate_parameters;

        std::vector<std::array<int16_t, 5> > mics;
        std::vector<char> matches;
        std::vector<char> original_matches;
        std::set<int> combed_frames;
        std::set<int> decimated_frames;
        std::vector<int> decimate_metrics;

        bool is_wobbly; // XXX Maybe only the json writing function needs to know.

        std::map<std::string, Preset> presets; // Key is Preset::name

        std::map<int, FreezeFrame> frozen_frames; // Key is FreezeFrame::first

        std::map<int, Section> sections[3]; // Key is Section::start

        std::vector<CustomList> custom_lists[3];

        Resize resize;
        Crop crop;


        // Only functions below.

        void writeProject(const std::string &path);
        void readProject(const std::string &path);


        void addFreezeFrame(int first, int last, int replacement);
        void deleteFreezeFrame(int frame);
        const FreezeFrame *findFreezeFrame(int frame);


        void addPreset(const std::string &preset_name);
        void addPreset(const std::string &preset_name, const std::string &preset_contents);
        void deletePreset(const std::string &preset_name);
        void assignPresetToSection(const std::string &preset_name, PositionInFilterChain position, int section_start);


        void setMatch(int frame, char match);


        void addSection(int section_start, PositionInFilterChain position);
        void addSection(const Section &section, PositionInFilterChain position);
        void deleteSection(int section_start, PositionInFilterChain position);
        const Section *findSection(int frame, PositionInFilterChain position);
        const Section *findNextSection(int frame, PositionInFilterChain position);


        void addCustomList(const std::string &list_name, PositionInFilterChain position);
        void addCustomList(const std::string &list_name, const std::string &list_preset, PositionInFilterChain position);
        void deleteCustomList(const std::string &list_name, PositionInFilterChain position); // XXX Maybe overload to take an index instead of name.


        void addDecimatedFrame(int frame);
        void deleteDecimatedFrame(int frame);
        bool isDecimatedFrame(int frame);


        void addCombedFrame(int frame);
        void deleteCombedFrame(int frame);
        bool isCombedFrame(int frame);


        std::string frameToTime(int frame);

        void sectionsToScript(std::string &script, PositionInFilterChain position);
        void customListsToScript(std::string &script, PositionInFilterChain position);
        void headerToScript(std::string &script);
        void presetsToScript(std::string &script);
        void sourceToScript(std::string &script);
        void trimToScript(std::string &script);
        void fieldHintToScript(std::string &script);
        void freezeFramesToScript(std::string &script);
        void decimatedFramesToScript(std::string &script);
        void cropToScript(std::string &script);
        void resizeToScript(std::string &script);
        void rgbConversionToScript(std::string &script);
        void setOutputToScript(std::string &script);

        std::string generateFinalScript();
        std::string generateMainDisplayScript();

    private:
        bool isNameSafeForPython(const std::string &name);
};

#endif // WOBBLYPROJECT_H