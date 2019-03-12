#include <mesh_map/roughness_layer.h>
#include <lvr2/algorithm/GeometryAlgorithms.hpp>
#include <lvr2/algorithm/NormalAlgorithms.hpp>

namespace mesh_map{

  bool RoughnessLayer::readLayer()
  {
    ROS_INFO_STREAM("Try to read roughness from map file...");
    auto roughness_opt = mesh_io_ptr->getDenseAttributeMap<lvr2::DenseVertexMap<float>>("roughness");
    if(roughness_opt){
      roughness = roughness_opt.get();
      return true;
    }
    else{
      return false;
    }
  }

  bool RoughnessLayer::writeLayer()
  {
    if(mesh_io_ptr->addDenseAttributeMap(roughness, "roughness"))
    {
      ROS_INFO_STREAM("Saved roughness to map file.");
      return true;
    }
    else
    {
      ROS_ERROR_STREAM("Could not save roughness to map file!");
      return false;
    }
  }

  float RoughnessLayer::getThreshold()
  {

  }

  bool RoughnessLayer::computeLayer(const MeshMapConfig& config)
  {
    ROS_INFO_STREAM("Computing roughness...");

    lvr2::DenseFaceMap<NormalType> face_normals;

    auto face_normals_opt =
        mesh_io_ptr->getDenseAttributeMap<lvr2::DenseFaceMap<NormalType>>("face_normals");

    if(face_normals_opt)
    {
      face_normals = face_normals_opt.get();
      ROS_INFO_STREAM("Found " << face_normals.numValues() << " face normals in map file.");
    }
    else
    {
      ROS_INFO_STREAM("No face normals found in the given map file, computing them...");
      face_normals = lvr2::calcFaceNormals(*mesh_ptr);
      ROS_INFO_STREAM("Computed "<< face_normals.numValues() << " face normals.");
      if(mesh_io_ptr->addDenseAttributeMap(face_normals, "face_normals"))
      {
        ROS_INFO_STREAM("Saved face normals to map file.");
      }
      else
      {
        ROS_ERROR_STREAM("Could not save face normals to map file!");
      }
    }

    lvr2::DenseVertexMap<NormalType> vertex_normals;
    auto vertex_normals_opt =
        mesh_io_ptr->getDenseAttributeMap<lvr2::DenseVertexMap<NormalType>>("vertex_normals");

    if(vertex_normals_opt)
    {
      vertex_normals = vertex_normals_opt.get();
      ROS_INFO_STREAM("Found "<< vertex_normals.numValues() << " vertex normals in map file!");
    }
    else
    {
      ROS_INFO_STREAM("No vertex normals found in the given map file, computing them...");
      vertex_normals = lvr2::calcVertexNormals(*mesh_ptr, face_normals);
      if(mesh_io_ptr->addDenseAttributeMap(vertex_normals, "vertex_normals"))
      {
        ROS_INFO_STREAM("Saved vertex normals to map file.");
      }
      else
      {
        ROS_ERROR_STREAM("Could not save vertex normals to map file!");
      }
    }

    roughness = lvr2::calcVertexRoughness(*mesh_ptr, config.roughness_radius, vertex_normals);
  }

  lvr2::VertexMap<float>& RoughnessLayer::getCosts()
  {
    return roughness;
  }

  const std::string RoughnessLayer::getName()
  {
    return "roughness";
  }

} /* namespace mesh_map */