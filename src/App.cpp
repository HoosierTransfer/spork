#include <App.hpp>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <Framebuffer.hpp>
#include <lenia/Lenia.hpp>
#include <lenia/LeniaTemplate.h>
#include <lenia/Parser.hpp>

#include <chrono>
#include <thread>

#include <vector>

void parse(std::string str, Grid& grid) {
    int x = 54;
    int y = 54;
    std::string cell = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ',') {
            grid.set(x, y, std::stod(cell));
            cell = "";
            x++;
        } else if (str[i] == '/') {
            grid.set(x, y, std::stod(cell));
            cell = "";
            x = 54;
            y++;
        } else {
            cell += str[i];
        }
    }
}

void App::run() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Lenia++", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return;
    }

    glViewport(0, 0, 1280, 720);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    LeniaTemplate leniaTemplate = ParseRule("R=18;k=quad4(1,11/12);d=quad4(0.32,0.051)*0.1;cells=0");

    leniaTemplate.coreId = 1;
    leniaTemplate.layerId = 3;
    leniaTemplate.kernel_B = {1.0/4.0, 1.0/2.0, 3.0/4.0, 1.0};
    leniaTemplate.alpha = 4;
    leniaTemplate.deltaId = 1;
    leniaTemplate.mu = 0.2;
    leniaTemplate.sigma = 0.02;
    leniaTemplate.kernelRadius = 18;
    leniaTemplate.updateFrequency = 10;

    Lenia lenia(150, 150, leniaTemplate);

    parse(
        "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.004623,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.009306,0.377848,0.540738,0.40135,0.102397,0,0.106441,0.269446,0.218586,0.006525,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.144874,0.429219,0.535085,0.458401,0.327106,0.423087,0.689118,0.752357,0.707958,0.699299,0.514508,0.408516,0.474658,0.483924,0.39178,0.122488,0.004492,0.01288,0.028742,0,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0,0,0,0,0,0.135634,0.445322,0.62519,0.6787,0.721175,0.799217,0.83123,0.788514,0.662204,0.517404,0.476013,0.621237,0.638103,0.54638,0.466562,0.43915,0.439998,0.210131,0.105709,0.118862,0.086788,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0,0.172105,0.375614,0.453363,0.505804,0.573862,0.634307,0.650596,0.659855,0.617697,0.406338,0.360494,0.533721,0.584166,0.487686,0.206974,0.04945,0.307844,0.471245,0.435857,0.365079,0.370365,0.435513,0.25924,0.210528,0.213767,0.103428,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0.029102,0.292943,0.392502,0.417781,0.354488,0.290547,0.361339,0.503201,0.622269,0.612025,0.106245,0.024685,0.048032,0.242856,0.356909,0.073526,0,0,0.195553,0.429026,0.374198,0.172132,0.278954,0.392212,0.299184,0.301405,0.272795,0.074746,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0.07325,0.185223,0.28875,0.344749,0.376035,0.45303,0.36962,0.180497,0.21434,0.211889,0.23843,0.174633,0.011676,0,0,0,0,0,0,0,0,0.234455,0.31495,0.024731,0,0.339715,0.393462,0.377439,0.3502,0.253939,0.022101,0,0,0,0,0,0,0/0,0,0,0,0.044042,0.149506,0.125106,0.025922,0,0.048157,0.15057,0.278123,0.2938,0.223427,0.21114,0.18715,0.13089,0.004958,0,0,0,0,0,0,0,0,0,0,0.133528,0,0,0.092338,0.465282,0.462444,0.364729,0.321598,0.145384,0,0,0,0,0,0,0/0,0,0,0,0.031978,0.129615,0.176491,0.01881,0,0,0,0.023495,0.193673,0.215329,0.22207,0.172119,0.036961,0,0,0.113238,0,0,0,0.144717,0.155347,0,0,0,0.167017,0,0,0,0.334537,0.475081,0.294094,0.256184,0.304995,0.052918,0.031718,0.024264,0,0,0,0/0,0,0,0,0.052237,0.101655,0.197526,0.161478,0,0,0,0.063637,0.202105,0.170165,0.446674,0.216263,0.002606,0,0.134079,0.977292,0.487684,0,0,0.635501,0.95224,0.193279,0,0.02944,0.558451,0.331406,0,0,0.239841,0.404173,0.168174,0.103379,0.366073,0.187566,0.107404,0.100371,0,0,0,0/0,0,0,0,0,0,0,0,0,0.023479,0.005307,0.109363,0.168194,0.103934,0.922615,1,0.003572,0,0,0.891283,0.751339,0,0,0.56351,0.983538,0.514244,0,0.241465,0.902647,0.73782,0.075673,0.010983,0.378851,0.466286,0.164885,0.023692,0.300496,0.349291,0.239932,0.206547,0.052282,0,0,0/0,0,0,0,0,0,0,0,0.051668,0.160116,0.518002,0.28388,0.158412,0.044397,0.519357,1,0.241665,0,0,0.490923,0.590488,0.076465,0,0.44808,0.892167,0.492156,0,0.33834,0.999732,0.841525,0.159316,0.123253,0.607522,0.631928,0.295722,0.079651,0.272277,0.402227,0.342456,0.279198,0.148208,0,0,0/0,0,0,0,0,0,0,0,0.082343,0.153002,0.839735,0.865387,0.17778,0.013239,0.071353,0.629661,0.324254,0,0.002907,0.422271,0.555208,0.457156,0.366059,0.657296,0.839649,0.650757,0.262334,0.577741,1,0.782936,0.164094,0.219321,0.756373,0.754316,0.410822,0.184993,0.335086,0.377247,0.341158,0.26268,0.229853,0,0,0/0,0,0,0,0,0,0,0,0.083835,0.188039,0.34873,0.742375,0.366629,0.003301,0.005183,0.44178,0.399354,0.283372,0.335451,0.446103,0.565206,0.612073,0.64836,0.751226,0.869355,0.884388,0.821438,0.962552,1,0.879207,0.380575,0.479003,0.830017,0.759119,0.382342,0.261325,0.442846,0.358525,0.316263,0.235767,0.246193,0.047708,0,0/0,0,0,0,0,0.006332,0.481629,0.426444,0.067187,0.212791,0.207462,0.481997,0.630057,0.143731,0.091327,0.251107,0.314666,0.308043,0.360611,0.447822,0.549704,0.619126,0.681931,0.778512,0.875395,0.915261,0.947047,1,1,1,0.820864,0.849898,0.866683,0.683627,0.301506,0.337662,0.534296,0.400302,0.319179,0.242289,0.241593,0.097429,0.008105,0/0,0,0,0,0,0,0.133536,0.433554,0.28572,0.239232,0.231601,0.480275,0.54337,0.157289,0.055976,0.121308,0.215748,0.257306,0.312916,0.393285,0.475952,0.559159,0.653275,0.759124,0.83914,0.896016,0.955775,1,1,1,1,0.939957,0.827919,0.661836,0.36962,0.476499,0.596468,0.46919,0.332622,0.259159,0.204115,0.091162,0.054603,0.015284/0,0,0,0,0,0,0,0,0.340109,0.438654,0.302107,0.311527,0.356353,0.146133,0,0.039325,0.111156,0.168367,0.251774,0.344846,0.432541,0.52006,0.613834,0.70963,0.789165,0.870497,0.95072,1,1,1,1,0.95458,0.845621,0.707882,0.586329,0.60911,0.576983,0.473054,0.331441,0.289531,0.154069,0.055596,0.069994,0.047547/0,0,0,0.082578,0,0,0,0,0.322612,0.445761,0.27237,0.147052,0.141629,0.059973,0,0,0.063511,0.14733,0.241083,0.338089,0.432707,0.528258,0.622172,0.721424,0.816426,0.897419,0.97426,1,1,1,1,0.983562,0.897878,0.787113,0.696626,0.579475,0.515622,0.397397,0.315906,0.321896,0.137072,0.050096,0.064598,0.043724/0,0,0,0.097861,0.14848,0.013366,0,0.1235,0.223601,0.328138,0.275651,0.150159,0.070427,0.005661,0,0,0.04465,0.134731,0.228145,0.324183,0.424867,0.524773,0.624491,0.726153,0.821329,0.917562,1,1,1,1,1,0.967609,0.910634,0.829771,0.711738,0.551422,0.473573,0.374758,0.357148,0.326818,0.153681,0.069759,0.052328,0.005143/0,0,0,0,0,0.06834,0.190212,0.137105,0.143136,0.210597,0.23969,0.156621,0.061517,0,0,0,0.011184,0.107482,0.196722,0.292462,0.401711,0.511791,0.619718,0.724693,0.822886,0.926868,1,1,1,0.950685,0.920491,0.917103,0.89501,0.815901,0.708937,0.589567,0.479774,0.405462,0.345433,0.267745,0.19318,0.123545,0.041726,0/0,0,0,0,0,0,0.083134,0.098713,0.125601,0.19685,0.252776,0.159028,0.035406,0,0,0,0,0.028391,0.137171,0.24902,0.365881,0.485798,0.625511,0.74139,0.838857,0.941523,1,1,1,0.94005,0.892939,0.865861,0.814353,0.744771,0.678088,0.598173,0.507846,0.374156,0.234485,0.209422,0.204665,0.149146,0.029674,0/0,0,0,0,0,0,0,0.021553,0.102476,0.202369,0.246542,0.177198,0.035856,0,0,0,0,0,0.166358,0.334578,0.488956,0.637144,0.767309,0.853538,0.921181,0.980365,1,1,1,1,0.942908,0.848181,0.755975,0.688474,0.63288,0.584383,0.50479,0.334906,0.202588,0.171839,0.175853,0.136418,0.012013,0/0,0,0,0,0,0,0,0,0.090779,0.203933,0.235147,0.170495,0.046744,0,0,0,0,0.117619,0.344742,0.512707,0.639499,0.770352,0.881874,0.943975,0.98642,1,1,1,1,1,1,0.863588,0.748134,0.649062,0.579666,0.518961,0.442092,0.335917,0.228758,0.159144,0.130323,0.068827,0,0/0,0,0,0,0,0,0,0,0.045291,0.195569,0.256859,0.166687,0.038694,0,0,0,0,0.112068,0.202898,0.288633,0.366607,0.428158,0.542866,0.685853,0.904842,1,1,1,1,1,1,0.988325,0.775867,0.631428,0.510044,0.431716,0.38471,0.327237,0.239165,0.127835,0.045142,0.011052,0,0/0,0,0,0,0,0,0,0,0.041433,0.205366,0.297721,0.188475,0.02814,0,0,0,0,0,0,0,0,0,0,0,0,0.256516,0.568135,0.762453,1,1,1,1,0.853455,0.609572,0.471593,0.366673,0.318551,0.286355,0.206422,0.062058,0,0,0,0/0,0,0,0,0,0,0,0,0.069596,0.274386,0.336842,0.194055,0.015155,0,0,0,0,0,0,0,0,0,0,0,0,0,0.012968,0.290699,0.424703,0.818569,1,1,1,0.595691,0.438737,0.306623,0.239326,0.21028,0.133746,0.032085,0,0,0,0/0,0,0,0,0,0,0,0,0.046709,0.317367,0.381798,0.175264,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.135187,0.296879,0.792262,1,1,0.843526,0.409774,0.269953,0.171026,0.120043,0.076376,0.019336,0,0,0,0/0,0,0,0,0,0,0,0,0.067582,0.394975,0.420044,0.123535,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.078419,0.306551,0.992975,1,1,0.374312,0.215981,0.105893,0.050715,0.039172,0,0,0,0,0/0,0,0,0,0,0,0,0,0.095458,0.477857,0.443728,0.038376,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.073245,0.472583,1,1,0.477533,0.162455,0.052511,0.014475,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0.177347,0.604777,0.410003,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.000363,0.148194,0.893728,1,0.620726,0.1065,0.020133,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0.301748,0.733106,0.400704,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.045544,0.621035,1,0.687615,0.050964,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0.368366,0.817198,0.454557,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.001031,0.451119,1,0.628,0.01379,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0.489538,0.930671,0.524424,0.014163,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.351995,0.903147,0.625819,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0.004646,0.526837,1,0.641422,0.051395,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.288251,0.770647,0.523432,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0.045832,0.479999,1,0.802161,0.117312,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.274719,0.677319,0.381075,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0.018836,0.101748,0.467206,1,1,0.318614,0.017247,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.300259,0.612476,0.285093,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0.017146,0.051994,0.15394,0.310206,1,1,0.774331,0.107378,0.007235,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.000418,0.34451,0.512001,0.170634,0,0,0,0,0,0,0,0,0/0,0,0,0,0.0358,0.068112,0.112488,0.204679,0.351541,0.878251,1,1,0.500526,0.107366,0.012433,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.038874,0.360124,0.417957,0.128659,0,0,0,0,0,0,0,0,0/0,0,0,0.016553,0.107294,0.150798,0.18564,0.259191,0.391389,0.603444,1,1,1,0.509007,0.208226,0.067014,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.114495,0.346543,0.337407,0.093642,0,0,0,0,0,0,0,0,0/0,0,0,0.055953,0.153765,0.219837,0.246789,0.31049,0.429733,0.588641,0.880697,1,1,1,0.793401,0.501977,0.258849,0,0,0,0,0,0,0,0,0,0,0,0,0,0.001637,0.148088,0.311216,0.26351,0.072389,0,0,0,0,0,0,0,0,0/0,0.004304,0.031541,0.068079,0.158561,0.257594,0.332311,0.389986,0.46856,0.585506,0.70943,1,1,1,1,1,0.842824,0.584423,0.176772,0,0,0,0,0,0,0,0,0,0,0,0.023117,0.15043,0.274665,0.233011,0.095698,0,0,0,0,0,0,0,0,0/0,0.056902,0.05952,0.064568,0.154121,0.3171,0.422969,0.468498,0.509401,0.590895,0.702882,0.845898,1,1,1,1,1,1,0.981411,0.904825,0.78309,0.657481,0.547297,0.438042,0.331467,0.224119,0.056167,0,0,0,0.022876,0.140504,0.251121,0.216591,0.092696,0,0,0,0,0,0,0,0,0/0.012934,0.073271,0.086206,0.091554,0.190459,0.349006,0.443031,0.511233,0.569118,0.635303,0.719899,0.806807,0.924589,1,1,1,1,1,0.983613,0.952942,0.902444,0.805651,0.675339,0.518047,0.318432,0.112676,0,0,0,0,0.022243,0.124156,0.234295,0.230551,0.101393,0,0,0,0,0,0,0,0,0/0.005647,0.066701,0.133408,0.186614,0.258941,0.352088,0.442163,0.540326,0.640325,0.71955,0.762226,0.790578,0.867573,0.996251,1,1,1,0.978536,0.903152,0.821731,0.723144,0.610699,0.478613,0.320838,0.162762,0.025359,0,0,0,0,0.042256,0.158957,0.231408,0.222057,0.100057,0,0,0,0.007889,0,0,0,0,0/0,0.051358,0.190041,0.280942,0.31739,0.346474,0.419493,0.562731,0.703973,0.768417,0.810182,0.827152,0.872073,0.936611,1,1,1,0.949731,0.849024,0.742765,0.632662,0.527885,0.410873,0.292745,0.175485,0.065725,0,0,0,0,0.04703,0.187947,0.231632,0.217765,0.154901,0.090549,0.01556,0,0,0,0.081318,0.065716,0,0/0,0.044154,0.218554,0.298529,0.333122,0.362708,0.404515,0.556883,0.695391,0.781403,0.850426,0.909088,0.970265,0.980259,0.980535,1,1,0.943425,0.851128,0.746495,0.635894,0.532146,0.427554,0.328374,0.228087,0.125219,0.034288,0,0,0,0.038036,0.166518,0.225774,0.210917,0.208578,0.227732,0.123234,0,0,0,0,0,0,0/0,0.055467,0.214333,0.298252,0.383636,0.428918,0.480861,0.552812,0.644822,0.771066,0.879239,0.983709,1,1,1,1,1,0.935029,0.850469,0.753071,0.649538,0.549045,0.450261,0.357353,0.260207,0.160364,0.070014,0,0,0,0.036697,0.193306,0.332626,0.265774,0.257256,0.315384,0.216391,0,0,0,0,0,0,0/0.016567,0.093285,0.20466,0.287134,0.384424,0.484363,0.479225,0.472987,0.606033,0.719517,0.862649,0.986517,1,1,1,1,0.964714,0.912245,0.834689,0.739863,0.639375,0.544999,0.452676,0.359703,0.264332,0.172985,0.088531,0.015862,0,0,0.029174,0.218167,0.46993,0.358675,0.152526,0.096259,0.075172,0.246369,0.12551,0,0,0,0,0/0.037893,0.156878,0.209825,0.25348,0.325273,0.4767,0.369545,0.365405,0.619124,0.716952,0.84983,0.937997,1,1,1,1,0.952468,0.895148,0.826783,0.720732,0.620514,0.545735,0.464314,0.37554,0.291899,0.222612,0.161195,0.088532,0.10909,0.172095,0.123565,0.235,0.561758,0.459892,0.044639,0.056979,0,0.002986,0.234704,0.026317,0,0,0,0/0.029765,0.190823,0.230002,0.235841,0.282774,0.413939,0.322977,0.429764,0.683839,0.761223,0.722176,0.786733,1,1,1,1,0.979026,0.945262,0.855936,0.744173,0.670951,0.606562,0.533286,0.451481,0.376385,0.299724,0.219179,0.151823,0.185489,0.405917,0.309217,0.152218,0.395631,0.490769,0.157378,0.009673,0,0,0,0,0,0,0,0/0,0.185604,0.24386,0.250356,0.296471,0.34925,0.302721,0.529042,0.723992,0.699223,0.391887,0.53244,0.961309,1,1,0.989134,0.993557,0.95078,0.845959,0.76218,0.723367,0.660158,0.554464,0.472017,0.420686,0.368612,0.239829,0.146015,0.213211,0.611878,0.342242,0.171492,0.229029,0.441837,0.618936,0.255119,0,0,0,0,0,0,0,0/0,0.141699,0.238284,0.256854,0.328824,0.273428,0.262628,0.513155,0.6729,0.590042,0.210182,0.475216,0.952248,0.973709,0.604245,0.642838,0.968123,0.925512,0.781209,0.712488,0.717995,0.663732,0.519465,0.430401,0.399502,0.431095,0.24476,0,0,0.644957,0.567578,0.212725,0.209208,0.135442,0.429103,0.43056,0,0,0,0,0,0,0,0/0,0.059686,0.149596,0.192331,0.337414,0.199768,0.183926,0.439859,0.571867,0.43847,0.181147,0.57202,0.988397,0.822527,0.174768,0.254019,0.867131,0.872046,0.381565,0.225051,0.563772,0.643189,0.285455,0,0.09492,0.500105,0.284096,0,0,0.641832,0.974054,0.500238,0.213643,0.119387,0,0,0,0,0,0,0,0,0,0/0,0,0.043655,0.061463,0.287415,0.226803,0.147848,0.369004,0.456989,0.239816,0.066113,0.519482,0.93534,0.738044,0.074104,0.200764,0.914651,0.862111,0.090246,0,0.412219,0.756609,0.174324,0,0,0.777071,0.831562,0,0,0.300915,0.947046,0.788523,0.223672,0.098024,0,0,0,0,0.01479,0.055179,0,0,0,0/0,0,0,0,0.145683,0.30073,0.2361,0.399157,0.427969,0.090856,0,0.215601,0.683002,0.501121,0,0.140075,0.915915,0.934902,0.048047,0,0.354105,0.952957,0.452469,0,0,0.779377,1,0.163876,0,0.169123,0.156843,0.166709,0.190094,0.060581,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0.032953,0.282903,0.310843,0.433378,0.48535,0.143527,0,0,0.322263,0.156397,0,0,0.465705,0.69842,0.002265,0,0,0.792533,0.471868,0,0,0.055185,0.559184,0.07055,0.044837,0.175286,0.12352,0.106733,0.123568,0.02174,0.017715,0,0,0,0,0,0.000652,0,0,0/0,0,0,0,0,0.170069,0.302715,0.337638,0.400848,0.341919,0,0,0.247908,0.092156,0,0,0,0.118894,0,0,0,0,0,0,0,0.033331,0.122388,0.092108,0.134161,0.187707,0.081017,0.071687,0.056005,0.210745,0.364132,0.243014,0.185789,0.121545,0,0,0,0,0,0/0,0,0,0,0,0.038661,0.226274,0.238061,0.244923,0.392944,0.150731,0.111464,0.389645,0.330945,0,0,0,0,0,0,0,0,0,0,0,0.061179,0.102168,0.140775,0.230676,0.267825,0.057761,0.045599,0.008608,0.082566,0.284285,0.266068,0.170404,0,0,0,0,0,0,0/0,0,0,0,0,0,0.090747,0.145379,0.139454,0.275146,0.347086,0.253171,0.43693,0.462226,0.211596,0,0,0.04752,0.135917,0,0,0,0,0.042394,0.154316,0.175899,0.166306,0.202871,0.458726,0.550515,0.443196,0.382689,0.33771,0.299665,0.287758,0.27702,0.141567,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0.056508,0.061558,0.11726,0.33352,0.381706,0.405928,0.432556,0.422571,0.170139,0,0.179766,0.401273,0.341455,0.002448,0,0.011319,0.451657,0.627732,0.53016,0.302512,0.238309,0.382966,0.50592,0.496298,0.456192,0.340513,0.135789,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0.018608,0.019806,0.105825,0.348016,0.403945,0.407506,0.449103,0.509171,0.353663,0.278584,0.413415,0.531127,0.56555,0.395289,0.292934,0.509097,0.670915,0.724596,0.747509,0.684908,0.586671,0.545543,0.519875,0.37057,0.037773,0,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0,0,0,0.072529,0.309885,0.383285,0.383965,0.420763,0.568888,0.510581,0.470975,0.597198,0.72618,0.787714,0.809582,0.719645,0.730173,0.751608,0.585562,0.340147,0.255145,0.293502,0.253242,0.054723,0,0,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0,0,0,0,0.023807,0.211983,0.239964,0.161082,0.209249,0.48069,0.606493,0.659958,0.574932,0.35245,0.367863,0.585861,0.677878,0.575845,0.263121,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.052576,0,0,0,0.173948,0.399634,0.309381,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"
    , lenia);

    Framebuffer framebuffer(1280, 720);

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    int fpsCount = 0;
    double fpsSum = 0.0;

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) { // If last print was more than 1 sec ago
            double fps = double(nbFrames) / (currentTime - lastTime);
            std::string windowTitle = "Lenia++ - FPS: " + std::to_string(fps);
            glfwSetWindowTitle(window, windowTitle.c_str());
            nbFrames = 0;
            lastTime = currentTime;

            fpsSum += fps;

            std::cout << "Average FPS: " << fpsSum / ++fpsCount << std::endl;
        }

        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        
        glClearColor(0.137, 0.09, 0.106, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::NewFrame();
        ImGui::Begin("Spork Viewer", nullptr, ImGuiWindowFlags_NoResize);

        ImGui::SetWindowSize(ImVec2(lenia.getWidth() * 3, lenia.getHeight() * 3));

        const float width = ImGui::GetContentRegionAvail().x;
        const float height = ImGui::GetContentRegionAvail().y;

        framebuffer.resize(width, height);
        glViewport(0, 0, width, height);

        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImGui::GetWindowDrawList()->AddImage(
            (void*)(intptr_t)framebuffer.getTexture(),
            pos,
            ImVec2(pos.x + width, pos.y + height),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        ImGui::Text("FPS: %.1f", io.Framerate);

        ImGui::End();

        ImGui::Begin("Controls", nullptr);
        if (ImGui::Button("Pause")) {
            paused = !paused;
        }

        if (ImGui::Button("Reset")) {
            lenia.reset();
                parse(
        "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.004623,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.009306,0.377848,0.540738,0.40135,0.102397,0,0.106441,0.269446,0.218586,0.006525,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.144874,0.429219,0.535085,0.458401,0.327106,0.423087,0.689118,0.752357,0.707958,0.699299,0.514508,0.408516,0.474658,0.483924,0.39178,0.122488,0.004492,0.01288,0.028742,0,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0,0,0,0,0,0.135634,0.445322,0.62519,0.6787,0.721175,0.799217,0.83123,0.788514,0.662204,0.517404,0.476013,0.621237,0.638103,0.54638,0.466562,0.43915,0.439998,0.210131,0.105709,0.118862,0.086788,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0,0.172105,0.375614,0.453363,0.505804,0.573862,0.634307,0.650596,0.659855,0.617697,0.406338,0.360494,0.533721,0.584166,0.487686,0.206974,0.04945,0.307844,0.471245,0.435857,0.365079,0.370365,0.435513,0.25924,0.210528,0.213767,0.103428,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0.029102,0.292943,0.392502,0.417781,0.354488,0.290547,0.361339,0.503201,0.622269,0.612025,0.106245,0.024685,0.048032,0.242856,0.356909,0.073526,0,0,0.195553,0.429026,0.374198,0.172132,0.278954,0.392212,0.299184,0.301405,0.272795,0.074746,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0.07325,0.185223,0.28875,0.344749,0.376035,0.45303,0.36962,0.180497,0.21434,0.211889,0.23843,0.174633,0.011676,0,0,0,0,0,0,0,0,0.234455,0.31495,0.024731,0,0.339715,0.393462,0.377439,0.3502,0.253939,0.022101,0,0,0,0,0,0,0/0,0,0,0,0.044042,0.149506,0.125106,0.025922,0,0.048157,0.15057,0.278123,0.2938,0.223427,0.21114,0.18715,0.13089,0.004958,0,0,0,0,0,0,0,0,0,0,0.133528,0,0,0.092338,0.465282,0.462444,0.364729,0.321598,0.145384,0,0,0,0,0,0,0/0,0,0,0,0.031978,0.129615,0.176491,0.01881,0,0,0,0.023495,0.193673,0.215329,0.22207,0.172119,0.036961,0,0,0.113238,0,0,0,0.144717,0.155347,0,0,0,0.167017,0,0,0,0.334537,0.475081,0.294094,0.256184,0.304995,0.052918,0.031718,0.024264,0,0,0,0/0,0,0,0,0.052237,0.101655,0.197526,0.161478,0,0,0,0.063637,0.202105,0.170165,0.446674,0.216263,0.002606,0,0.134079,0.977292,0.487684,0,0,0.635501,0.95224,0.193279,0,0.02944,0.558451,0.331406,0,0,0.239841,0.404173,0.168174,0.103379,0.366073,0.187566,0.107404,0.100371,0,0,0,0/0,0,0,0,0,0,0,0,0,0.023479,0.005307,0.109363,0.168194,0.103934,0.922615,1,0.003572,0,0,0.891283,0.751339,0,0,0.56351,0.983538,0.514244,0,0.241465,0.902647,0.73782,0.075673,0.010983,0.378851,0.466286,0.164885,0.023692,0.300496,0.349291,0.239932,0.206547,0.052282,0,0,0/0,0,0,0,0,0,0,0,0.051668,0.160116,0.518002,0.28388,0.158412,0.044397,0.519357,1,0.241665,0,0,0.490923,0.590488,0.076465,0,0.44808,0.892167,0.492156,0,0.33834,0.999732,0.841525,0.159316,0.123253,0.607522,0.631928,0.295722,0.079651,0.272277,0.402227,0.342456,0.279198,0.148208,0,0,0/0,0,0,0,0,0,0,0,0.082343,0.153002,0.839735,0.865387,0.17778,0.013239,0.071353,0.629661,0.324254,0,0.002907,0.422271,0.555208,0.457156,0.366059,0.657296,0.839649,0.650757,0.262334,0.577741,1,0.782936,0.164094,0.219321,0.756373,0.754316,0.410822,0.184993,0.335086,0.377247,0.341158,0.26268,0.229853,0,0,0/0,0,0,0,0,0,0,0,0.083835,0.188039,0.34873,0.742375,0.366629,0.003301,0.005183,0.44178,0.399354,0.283372,0.335451,0.446103,0.565206,0.612073,0.64836,0.751226,0.869355,0.884388,0.821438,0.962552,1,0.879207,0.380575,0.479003,0.830017,0.759119,0.382342,0.261325,0.442846,0.358525,0.316263,0.235767,0.246193,0.047708,0,0/0,0,0,0,0,0.006332,0.481629,0.426444,0.067187,0.212791,0.207462,0.481997,0.630057,0.143731,0.091327,0.251107,0.314666,0.308043,0.360611,0.447822,0.549704,0.619126,0.681931,0.778512,0.875395,0.915261,0.947047,1,1,1,0.820864,0.849898,0.866683,0.683627,0.301506,0.337662,0.534296,0.400302,0.319179,0.242289,0.241593,0.097429,0.008105,0/0,0,0,0,0,0,0.133536,0.433554,0.28572,0.239232,0.231601,0.480275,0.54337,0.157289,0.055976,0.121308,0.215748,0.257306,0.312916,0.393285,0.475952,0.559159,0.653275,0.759124,0.83914,0.896016,0.955775,1,1,1,1,0.939957,0.827919,0.661836,0.36962,0.476499,0.596468,0.46919,0.332622,0.259159,0.204115,0.091162,0.054603,0.015284/0,0,0,0,0,0,0,0,0.340109,0.438654,0.302107,0.311527,0.356353,0.146133,0,0.039325,0.111156,0.168367,0.251774,0.344846,0.432541,0.52006,0.613834,0.70963,0.789165,0.870497,0.95072,1,1,1,1,0.95458,0.845621,0.707882,0.586329,0.60911,0.576983,0.473054,0.331441,0.289531,0.154069,0.055596,0.069994,0.047547/0,0,0,0.082578,0,0,0,0,0.322612,0.445761,0.27237,0.147052,0.141629,0.059973,0,0,0.063511,0.14733,0.241083,0.338089,0.432707,0.528258,0.622172,0.721424,0.816426,0.897419,0.97426,1,1,1,1,0.983562,0.897878,0.787113,0.696626,0.579475,0.515622,0.397397,0.315906,0.321896,0.137072,0.050096,0.064598,0.043724/0,0,0,0.097861,0.14848,0.013366,0,0.1235,0.223601,0.328138,0.275651,0.150159,0.070427,0.005661,0,0,0.04465,0.134731,0.228145,0.324183,0.424867,0.524773,0.624491,0.726153,0.821329,0.917562,1,1,1,1,1,0.967609,0.910634,0.829771,0.711738,0.551422,0.473573,0.374758,0.357148,0.326818,0.153681,0.069759,0.052328,0.005143/0,0,0,0,0,0.06834,0.190212,0.137105,0.143136,0.210597,0.23969,0.156621,0.061517,0,0,0,0.011184,0.107482,0.196722,0.292462,0.401711,0.511791,0.619718,0.724693,0.822886,0.926868,1,1,1,0.950685,0.920491,0.917103,0.89501,0.815901,0.708937,0.589567,0.479774,0.405462,0.345433,0.267745,0.19318,0.123545,0.041726,0/0,0,0,0,0,0,0.083134,0.098713,0.125601,0.19685,0.252776,0.159028,0.035406,0,0,0,0,0.028391,0.137171,0.24902,0.365881,0.485798,0.625511,0.74139,0.838857,0.941523,1,1,1,0.94005,0.892939,0.865861,0.814353,0.744771,0.678088,0.598173,0.507846,0.374156,0.234485,0.209422,0.204665,0.149146,0.029674,0/0,0,0,0,0,0,0,0.021553,0.102476,0.202369,0.246542,0.177198,0.035856,0,0,0,0,0,0.166358,0.334578,0.488956,0.637144,0.767309,0.853538,0.921181,0.980365,1,1,1,1,0.942908,0.848181,0.755975,0.688474,0.63288,0.584383,0.50479,0.334906,0.202588,0.171839,0.175853,0.136418,0.012013,0/0,0,0,0,0,0,0,0,0.090779,0.203933,0.235147,0.170495,0.046744,0,0,0,0,0.117619,0.344742,0.512707,0.639499,0.770352,0.881874,0.943975,0.98642,1,1,1,1,1,1,0.863588,0.748134,0.649062,0.579666,0.518961,0.442092,0.335917,0.228758,0.159144,0.130323,0.068827,0,0/0,0,0,0,0,0,0,0,0.045291,0.195569,0.256859,0.166687,0.038694,0,0,0,0,0.112068,0.202898,0.288633,0.366607,0.428158,0.542866,0.685853,0.904842,1,1,1,1,1,1,0.988325,0.775867,0.631428,0.510044,0.431716,0.38471,0.327237,0.239165,0.127835,0.045142,0.011052,0,0/0,0,0,0,0,0,0,0,0.041433,0.205366,0.297721,0.188475,0.02814,0,0,0,0,0,0,0,0,0,0,0,0,0.256516,0.568135,0.762453,1,1,1,1,0.853455,0.609572,0.471593,0.366673,0.318551,0.286355,0.206422,0.062058,0,0,0,0/0,0,0,0,0,0,0,0,0.069596,0.274386,0.336842,0.194055,0.015155,0,0,0,0,0,0,0,0,0,0,0,0,0,0.012968,0.290699,0.424703,0.818569,1,1,1,0.595691,0.438737,0.306623,0.239326,0.21028,0.133746,0.032085,0,0,0,0/0,0,0,0,0,0,0,0,0.046709,0.317367,0.381798,0.175264,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.135187,0.296879,0.792262,1,1,0.843526,0.409774,0.269953,0.171026,0.120043,0.076376,0.019336,0,0,0,0/0,0,0,0,0,0,0,0,0.067582,0.394975,0.420044,0.123535,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.078419,0.306551,0.992975,1,1,0.374312,0.215981,0.105893,0.050715,0.039172,0,0,0,0,0/0,0,0,0,0,0,0,0,0.095458,0.477857,0.443728,0.038376,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.073245,0.472583,1,1,0.477533,0.162455,0.052511,0.014475,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0.177347,0.604777,0.410003,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.000363,0.148194,0.893728,1,0.620726,0.1065,0.020133,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0.301748,0.733106,0.400704,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.045544,0.621035,1,0.687615,0.050964,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0.368366,0.817198,0.454557,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.001031,0.451119,1,0.628,0.01379,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0.489538,0.930671,0.524424,0.014163,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.351995,0.903147,0.625819,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0.004646,0.526837,1,0.641422,0.051395,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.288251,0.770647,0.523432,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0.045832,0.479999,1,0.802161,0.117312,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.274719,0.677319,0.381075,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0.018836,0.101748,0.467206,1,1,0.318614,0.017247,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.300259,0.612476,0.285093,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0.017146,0.051994,0.15394,0.310206,1,1,0.774331,0.107378,0.007235,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.000418,0.34451,0.512001,0.170634,0,0,0,0,0,0,0,0,0/0,0,0,0,0.0358,0.068112,0.112488,0.204679,0.351541,0.878251,1,1,0.500526,0.107366,0.012433,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.038874,0.360124,0.417957,0.128659,0,0,0,0,0,0,0,0,0/0,0,0,0.016553,0.107294,0.150798,0.18564,0.259191,0.391389,0.603444,1,1,1,0.509007,0.208226,0.067014,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.114495,0.346543,0.337407,0.093642,0,0,0,0,0,0,0,0,0/0,0,0,0.055953,0.153765,0.219837,0.246789,0.31049,0.429733,0.588641,0.880697,1,1,1,0.793401,0.501977,0.258849,0,0,0,0,0,0,0,0,0,0,0,0,0,0.001637,0.148088,0.311216,0.26351,0.072389,0,0,0,0,0,0,0,0,0/0,0.004304,0.031541,0.068079,0.158561,0.257594,0.332311,0.389986,0.46856,0.585506,0.70943,1,1,1,1,1,0.842824,0.584423,0.176772,0,0,0,0,0,0,0,0,0,0,0,0.023117,0.15043,0.274665,0.233011,0.095698,0,0,0,0,0,0,0,0,0/0,0.056902,0.05952,0.064568,0.154121,0.3171,0.422969,0.468498,0.509401,0.590895,0.702882,0.845898,1,1,1,1,1,1,0.981411,0.904825,0.78309,0.657481,0.547297,0.438042,0.331467,0.224119,0.056167,0,0,0,0.022876,0.140504,0.251121,0.216591,0.092696,0,0,0,0,0,0,0,0,0/0.012934,0.073271,0.086206,0.091554,0.190459,0.349006,0.443031,0.511233,0.569118,0.635303,0.719899,0.806807,0.924589,1,1,1,1,1,0.983613,0.952942,0.902444,0.805651,0.675339,0.518047,0.318432,0.112676,0,0,0,0,0.022243,0.124156,0.234295,0.230551,0.101393,0,0,0,0,0,0,0,0,0/0.005647,0.066701,0.133408,0.186614,0.258941,0.352088,0.442163,0.540326,0.640325,0.71955,0.762226,0.790578,0.867573,0.996251,1,1,1,0.978536,0.903152,0.821731,0.723144,0.610699,0.478613,0.320838,0.162762,0.025359,0,0,0,0,0.042256,0.158957,0.231408,0.222057,0.100057,0,0,0,0.007889,0,0,0,0,0/0,0.051358,0.190041,0.280942,0.31739,0.346474,0.419493,0.562731,0.703973,0.768417,0.810182,0.827152,0.872073,0.936611,1,1,1,0.949731,0.849024,0.742765,0.632662,0.527885,0.410873,0.292745,0.175485,0.065725,0,0,0,0,0.04703,0.187947,0.231632,0.217765,0.154901,0.090549,0.01556,0,0,0,0.081318,0.065716,0,0/0,0.044154,0.218554,0.298529,0.333122,0.362708,0.404515,0.556883,0.695391,0.781403,0.850426,0.909088,0.970265,0.980259,0.980535,1,1,0.943425,0.851128,0.746495,0.635894,0.532146,0.427554,0.328374,0.228087,0.125219,0.034288,0,0,0,0.038036,0.166518,0.225774,0.210917,0.208578,0.227732,0.123234,0,0,0,0,0,0,0/0,0.055467,0.214333,0.298252,0.383636,0.428918,0.480861,0.552812,0.644822,0.771066,0.879239,0.983709,1,1,1,1,1,0.935029,0.850469,0.753071,0.649538,0.549045,0.450261,0.357353,0.260207,0.160364,0.070014,0,0,0,0.036697,0.193306,0.332626,0.265774,0.257256,0.315384,0.216391,0,0,0,0,0,0,0/0.016567,0.093285,0.20466,0.287134,0.384424,0.484363,0.479225,0.472987,0.606033,0.719517,0.862649,0.986517,1,1,1,1,0.964714,0.912245,0.834689,0.739863,0.639375,0.544999,0.452676,0.359703,0.264332,0.172985,0.088531,0.015862,0,0,0.029174,0.218167,0.46993,0.358675,0.152526,0.096259,0.075172,0.246369,0.12551,0,0,0,0,0/0.037893,0.156878,0.209825,0.25348,0.325273,0.4767,0.369545,0.365405,0.619124,0.716952,0.84983,0.937997,1,1,1,1,0.952468,0.895148,0.826783,0.720732,0.620514,0.545735,0.464314,0.37554,0.291899,0.222612,0.161195,0.088532,0.10909,0.172095,0.123565,0.235,0.561758,0.459892,0.044639,0.056979,0,0.002986,0.234704,0.026317,0,0,0,0/0.029765,0.190823,0.230002,0.235841,0.282774,0.413939,0.322977,0.429764,0.683839,0.761223,0.722176,0.786733,1,1,1,1,0.979026,0.945262,0.855936,0.744173,0.670951,0.606562,0.533286,0.451481,0.376385,0.299724,0.219179,0.151823,0.185489,0.405917,0.309217,0.152218,0.395631,0.490769,0.157378,0.009673,0,0,0,0,0,0,0,0/0,0.185604,0.24386,0.250356,0.296471,0.34925,0.302721,0.529042,0.723992,0.699223,0.391887,0.53244,0.961309,1,1,0.989134,0.993557,0.95078,0.845959,0.76218,0.723367,0.660158,0.554464,0.472017,0.420686,0.368612,0.239829,0.146015,0.213211,0.611878,0.342242,0.171492,0.229029,0.441837,0.618936,0.255119,0,0,0,0,0,0,0,0/0,0.141699,0.238284,0.256854,0.328824,0.273428,0.262628,0.513155,0.6729,0.590042,0.210182,0.475216,0.952248,0.973709,0.604245,0.642838,0.968123,0.925512,0.781209,0.712488,0.717995,0.663732,0.519465,0.430401,0.399502,0.431095,0.24476,0,0,0.644957,0.567578,0.212725,0.209208,0.135442,0.429103,0.43056,0,0,0,0,0,0,0,0/0,0.059686,0.149596,0.192331,0.337414,0.199768,0.183926,0.439859,0.571867,0.43847,0.181147,0.57202,0.988397,0.822527,0.174768,0.254019,0.867131,0.872046,0.381565,0.225051,0.563772,0.643189,0.285455,0,0.09492,0.500105,0.284096,0,0,0.641832,0.974054,0.500238,0.213643,0.119387,0,0,0,0,0,0,0,0,0,0/0,0,0.043655,0.061463,0.287415,0.226803,0.147848,0.369004,0.456989,0.239816,0.066113,0.519482,0.93534,0.738044,0.074104,0.200764,0.914651,0.862111,0.090246,0,0.412219,0.756609,0.174324,0,0,0.777071,0.831562,0,0,0.300915,0.947046,0.788523,0.223672,0.098024,0,0,0,0,0.01479,0.055179,0,0,0,0/0,0,0,0,0.145683,0.30073,0.2361,0.399157,0.427969,0.090856,0,0.215601,0.683002,0.501121,0,0.140075,0.915915,0.934902,0.048047,0,0.354105,0.952957,0.452469,0,0,0.779377,1,0.163876,0,0.169123,0.156843,0.166709,0.190094,0.060581,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0.032953,0.282903,0.310843,0.433378,0.48535,0.143527,0,0,0.322263,0.156397,0,0,0.465705,0.69842,0.002265,0,0,0.792533,0.471868,0,0,0.055185,0.559184,0.07055,0.044837,0.175286,0.12352,0.106733,0.123568,0.02174,0.017715,0,0,0,0,0,0.000652,0,0,0/0,0,0,0,0,0.170069,0.302715,0.337638,0.400848,0.341919,0,0,0.247908,0.092156,0,0,0,0.118894,0,0,0,0,0,0,0,0.033331,0.122388,0.092108,0.134161,0.187707,0.081017,0.071687,0.056005,0.210745,0.364132,0.243014,0.185789,0.121545,0,0,0,0,0,0/0,0,0,0,0,0.038661,0.226274,0.238061,0.244923,0.392944,0.150731,0.111464,0.389645,0.330945,0,0,0,0,0,0,0,0,0,0,0,0.061179,0.102168,0.140775,0.230676,0.267825,0.057761,0.045599,0.008608,0.082566,0.284285,0.266068,0.170404,0,0,0,0,0,0,0/0,0,0,0,0,0,0.090747,0.145379,0.139454,0.275146,0.347086,0.253171,0.43693,0.462226,0.211596,0,0,0.04752,0.135917,0,0,0,0,0.042394,0.154316,0.175899,0.166306,0.202871,0.458726,0.550515,0.443196,0.382689,0.33771,0.299665,0.287758,0.27702,0.141567,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0.056508,0.061558,0.11726,0.33352,0.381706,0.405928,0.432556,0.422571,0.170139,0,0.179766,0.401273,0.341455,0.002448,0,0.011319,0.451657,0.627732,0.53016,0.302512,0.238309,0.382966,0.50592,0.496298,0.456192,0.340513,0.135789,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0.018608,0.019806,0.105825,0.348016,0.403945,0.407506,0.449103,0.509171,0.353663,0.278584,0.413415,0.531127,0.56555,0.395289,0.292934,0.509097,0.670915,0.724596,0.747509,0.684908,0.586671,0.545543,0.519875,0.37057,0.037773,0,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0,0,0,0.072529,0.309885,0.383285,0.383965,0.420763,0.568888,0.510581,0.470975,0.597198,0.72618,0.787714,0.809582,0.719645,0.730173,0.751608,0.585562,0.340147,0.255145,0.293502,0.253242,0.054723,0,0,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0,0,0,0,0.023807,0.211983,0.239964,0.161082,0.209249,0.48069,0.606493,0.659958,0.574932,0.35245,0.367863,0.585861,0.677878,0.575845,0.263121,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0/0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.052576,0,0,0,0.173948,0.399634,0.309381,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"
    , lenia);
        }
        if (ImGui::Button("Randomize")) {
            lenia.randomize();
        }

        if (ImGui::Button("Step")) {
            lenia.update();
        }

        // show if its paused
        if (paused) {
            ImGui::Text("Paused");
        }

        if (ImGui::CollapsingHeader("Randomizer")) {
            ImGui::SliderInt("Patches", &lenia.randomPatches, 1, 10);
            ImGui::SliderInt("Patch Size", &lenia.randomPatchSize, 1, 30);
            if (ImGui::Button("Randomize Patches")) {
                lenia.randomizePatches();
            }
        }

        ImGui::End();

        ImGui::Begin("Simulation Parameters", nullptr);
        double min = 0.0;
        double max = 0.3;
        ImGui::SliderScalar("mu", ImGuiDataType_Double, &lenia.mu, &min, &max);

        min = 0.0;
        max = 0.1;
        
        ImGui::SliderScalar("sigma", ImGuiDataType_Double, &lenia.sigma, &min, &max);

        ImGui::SliderInt("Update Frequency", &lenia.updateFrequency, 1, 100);

        ImGui::End();

        ImGui::Render();

        framebuffer.bind();
        framebuffer.clear();

        lenia.draw();

        framebuffer.unbind();
        
        if (!paused) {
            lenia.update();
        }

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}