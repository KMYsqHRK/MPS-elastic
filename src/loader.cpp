#include "loader.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <yaml-cpp/yaml.h>

using std::cerr;
using std::cout;
using std::endl;
namespace fs = std::filesystem;

Input Loader::load(const fs::path& settingPath, const fs::path& outputDirectory) {
    Input input;
    input.settings = loadSettingYaml(settingPath);

    auto [startTime, particles] = loadParticleProf(input.settings.profPath);
    input.startTime             = startTime;
    input.particles             = particles;

    auto outputSettingPath = outputDirectory / settingPath.filename();
    if (fs::exists(outputSettingPath)) {
        cerr << "setting file already exists in the output directory: " << outputSettingPath << endl;
        std::exit(-1);
    } else {
        fs::copy_file(settingPath, outputSettingPath);
    }
    auto outputProfPath = outputDirectory / input.settings.profPath.filename();
    if (fs::exists(outputProfPath)) {
        cerr << "prof file already exists in the output directory: " << outputProfPath << endl;
        std::exit(-1);
    } else {
        fs::copy_file(input.settings.profPath, outputProfPath);
    }
    return input;
}

Settings Loader::loadSettingYaml(const fs::path& settingPath) {
    YAML::Node yaml = YAML::LoadFile(settingPath.string());

    Settings s;

    // computational conditions
    s.dim              = yaml["dim"].as<int>();
    s.particleDistance = yaml["particleDistance"].as<double>();
    s.dt               = yaml["dt"].as<double>();
    s.endTime          = yaml["endTime"].as<double>();
    s.outputPeriod     = yaml["outputPeriod"].as<double>();
    s.cflCondition     = yaml["cflCondition"].as<double>();
    s.numPhysicalCores = yaml["numPhysicalCores"].as<int>();

    // physical properties
    s.elasticdensity       = yaml["elasticdensity"].as<double>();
    s.elasticratio         = yaml["elasticratio"].as<double>();
    s.poissonratio         = yaml["poissonratio"].as<double>();
    s.lame1                = s.elasticratio * s.poissonratio / ((1 + s.poissonratio)*(1 - 2*s.poissonratio));
    s.lame2                = s.elasticratio / 2 * (1 + s.poissonratio);

    // gravity
    s.gravity[0] = yaml["gravity"][0].as<double>();
    s.gravity[1] = yaml["gravity"][1].as<double>();
    s.gravity[2] = yaml["gravity"][2].as<double>();


    // collision
    s.collisionDistance        = yaml["collisionDistanceRatio"].as<double>() * s.particleDistance;
    s.coefficientOfRestitution = yaml["coefficientOfRestitution"].as<double>();

    // effective radius
    s.re_forDivergence    = yaml["radiusRatioForDivergence"].as<double>() * s.particleDistance;
    s.re_forGradient      = yaml["radiusRatioForGradient"].as<double>() * s.particleDistance;
    s.re_forLaplacian     = yaml["radiusRatioForLaplacian"].as<double>() * s.particleDistance;
    s.reMax               = std::max({s.re_forDivergence, s.re_forGradient, s.re_forLaplacian});

    // domain
    s.domain.xMin    = yaml["domainMin"][0].as<double>();
    s.domain.xMax    = yaml["domainMax"][0].as<double>();
    s.domain.yMin    = yaml["domainMin"][1].as<double>();
    s.domain.yMax    = yaml["domainMax"][1].as<double>();
    s.domain.zMin    = yaml["domainMin"][2].as<double>();
    s.domain.zMax    = yaml["domainMax"][2].as<double>();
    s.domain.xLength = s.domain.xMax - s.domain.xMin;
    s.domain.yLength = s.domain.yMax - s.domain.yMin;
    s.domain.zLength = s.domain.zMax - s.domain.zMin;

    // profpath
    auto yamlDir          = settingPath.parent_path();
    auto relativeProfPath = yaml["profPath"].as<std::string>();
    s.profPath            = fs::weakly_canonical(yamlDir / relativeProfPath);

    return s;
}

std::pair<double, std::vector<Particle>> Loader::loadParticleProf(const fs::path& profPath) {
    std::ifstream ifs;
    ifs.open(profPath);
    if (ifs.fail()) {
        cerr << "cannot read prof file: " << fs::absolute(profPath) << endl;
        std::exit(-1);
    }

    std::vector<Particle> particles;
    double startTime = NAN;
    int particleSize = 0;
    ifs >> startTime;
    ifs >> particleSize;
    for (int i = 0; i < particleSize; i++) {
        int type_int = 0;
        ParticleType type;
        Eigen::Vector3d pos, vel;
        double ang, angularvel;

        ifs >> type_int;
        ifs >> pos.x() >> pos.y() >> pos.z();
        ifs >> vel.x() >> vel.y() >> vel.z();
        ifs >> ang >> angularvel;

        type = static_cast<ParticleType>(type_int);
        if (type != ParticleType::Ghost) {
            particles.emplace_back(particles.size(), type, pos, vel, ang, angularvel);
        }
    }
    ifs.close();
    ifs.clear();

    return {startTime, particles};
}
