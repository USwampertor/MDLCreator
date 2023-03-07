/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file 	main.cpp
 * @author 	Marco "Swampy" Millan
 * @date 	2022/09/06
 * @brief main file
 *
 */
 /*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#include <cppfs/FileHandle.h>
#include <cppfs/FilePath.h>
#include <cppfs/fs.h>
#include <cpplocate/cpplocate.h>
#include <iostream>
#include <string>
#include <vector>

struct MaterialInfo
{
  std::string m_id;
  std::string m_name;
  std::string m_description;
  std::string m_version;
  std::string m_author;
  std::vector<std::string> m_keywords;
  std::string m_diffuse;
  std::string m_normal;
  std::string m_albedo;
  std::string m_metal;
  std::string m_orm;
  std::string m_roughness;
  std::string m_emissive;
  std::string m_ao;

  void dummyTest() {
    cppfs::FilePath p(cpplocate::getModulePath());

    std::string folder = "./Test/";
    std::cout << folder;
    m_id = "Test1Mat";
    m_name = "Test Material";
    m_description = "A test material MDL creation";
    m_version = "1";
    m_author = "Swampertor";
    m_keywords.push_back("test");
    m_keywords.push_back("PBR");
    m_keywords.push_back("omniverse");
    m_keywords.push_back("default");
    m_diffuse = folder + "BC.png";
    m_normal = folder + "NM_version2.png";
    m_metal = folder + "Metal.png";
    m_roughness = folder + "Rou.png";
    m_emissive = folder + "E.png";
    m_ao = folder + "AO.png";
  }
};

void createMDL( MaterialInfo info) {
  std::string materialTemplate =
    "\
mdl 1.4; \n\n\
import ::OmniPBR::OmniPBR; \n\
import ::anno::author; \n\
import ::anno::description; \n\
import ::anno::display_name; \n\
import ::anno::key_words; \n\
import ::anno::version; \n\
import ::tex::gamma_mode; \n\
import ::state::normal; \n\
\n";
  materialTemplate +=
    "export material " + info.m_id + "(*) \n\
[[ \n\
    ::anno::display_name(\"" + info.m_name + "\"), \n\
    ::anno::description(\"" + info.m_description + "\"), \n\
    ::anno::version(1, 0, 0, \"\"), \n\
    ::anno::author(\"" + info.m_author + "\"), \n\
    ::anno::key_words(string[]("; 
  
  for (int i = 0; i < info.m_keywords.size(); ++i) {
    materialTemplate += "\"" + info.m_keywords[i] +"\"";
    if (i != info.m_keywords.size() - 1) {
      materialTemplate += ",";
    }
  }
    // omni", "PBR", "omniverse", "generic"
    
  materialTemplate += ")) \n\
]] \n\
 = ::OmniPBR::OmniPBR( \n\
    diffuse_color_constant: color(0.200000003f, 0.200000003f, 0.200000003f), \n\
    diffuse_texture: texture_2d(" +
    (info.m_diffuse.empty() ?
      "" :
      ("\"" + info.m_diffuse + "\" /* tag 2828, version 6332211 */, ::tex::gamma_srgb")) +
    "), \n\
    albedo_desaturation: 0.f, \n\
    albedo_add: 0.f, \n\
    albedo_brightness: 1.f, \n\
    diffuse_tint: color(1.f, 1.f, 1.f), \n\
    reflection_roughness_constant: 0.5f, \n\
    reflection_roughness_texture_influence: 1.f, \n\
    reflectionroughness_texture: texture_2d(" +

    (info.m_roughness.empty() ?
      "" :
      ("\"" + info.m_roughness + "\" /* tag 2830, version 596885211 */, ::tex::gamma_linear")) +
    "), \n\
    metallic_constant: 0.f,\n\
    metallic_texture_influence: 1.f, \n\
    metallic_texture: texture_2d(" +
    (info.m_metal.empty() ?
      "" :
      ("\"" + info.m_metal + "\" /* tag 2830, version 596885211 */, ::tex::gamma_linear")) +
    "), \n\
    specular_level: 0.5f, \n\
    enable_ORM_texture: true, \n\
    ORM_texture: texture_2d(" +
    (info.m_orm.empty() ?
      "" :
      ("\"" + info.m_orm + "\" /* tag 2830, version 596885211 */, ::tex::gamma_linear")) +
    "), \n\
    ao_to_diffuse: 0.f, \n\
    ao_texture: texture_2d(" +
    (info.m_ao.empty() ?
      "" :
      ("\"" + info.m_ao + "\" /* tag 2830, version 596885211 */, ::tex::gamma_linear")) +
    "), \n\
    enable_emission: false, \n\
    emissive_color: color(1.f, 0.100000001f, 0.100000001f), \n\
    emissive_mask_texture: texture_2d(" +
    (info.m_emissive.empty() ? 
      "" : 
      ("\"" + info.m_emissive + "\" /* tag 2828, version 6332211 */, ::tex::gamma_srgb")) +
    "), \n\
    emissive_intensity: 40.f, \n\
    bump_factor: 1.f, \n\
    normalmap_texture: texture_2d(" +
    (info.m_normal.empty() ? 
      "" : 
      ("\""+ info.m_normal +"\" /* tag 2832, version 3494456508 */, ::tex::gamma_linear")) +
    "), \n\
    detail_bump_factor: 0.300000012f, \n\
    detail_normalmap_texture: texture_2d(), \n\
    project_uvw: false, \n\
    world_or_object: false, \n\
    uv_space_index: 0, \n\
    texture_translate: float2(0.f), \n\
    texture_rotate: 0.f, \n\
    texture_scale: float2(1.f), \n\
    detail_texture_translate: float2(0.f), \n\
    detail_texture_rotate: 0.f, \n\
    detail_texture_scale: float2(1.f)); \n\
";

  cppfs::FileHandle newMaterial = cppfs::fs::open(cpplocate::getModulePath()+ "/" + info.m_id + ".mdl");
  if (newMaterial.exists()) { newMaterial.remove(); }
  newMaterial.writeFile(materialTemplate);
}


int main() {
  MaterialInfo info;
  info.dummyTest();
  createMDL(info);
}