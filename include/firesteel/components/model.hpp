#ifndef FS_COMP_MODEL_H
#define FS_COMP_MODEL_H
#include <firesteel/common.hpp>
#include <firesteel/component.hpp>
#include <firesteel/entity.hpp>
#include <firesteel/enviroment.hpp>
#include <firesteel/utils/delta_time.hpp>
#include <firesteel/rendering/mesh.hpp>

#ifdef FS_LOADER_OBJ
#include <firesteel/loaders/obj.hpp>
#endif // FS_LOADER_OBJ
#ifdef FS_LOADER_GLTF
#include <firesteel/loaders/gltf.hpp>
#endif // FS_LOADER_GLTF
#ifdef FS_LOADER_FBX
#include <firesteel/loaders/fbx.hpp>
#endif // FS_LOADER_FBX

namespace Firesteel {
	class ModelComponent : public Component {
	public:
		ModelComponent(std::shared_ptr<Entity> tEntity, const std::string& tPath="")
			: path(tPath), Component(tEntity) {
                model.materials.clear();
                model.nodes.clear();
                model.meshes.clear();
        }
        void onStart() override {

        }
		void onDraw() override {
		}
        void onRemove() override {
            model.materials.clear();
            removeModel();
        }

		void properties() override {
			PROPERTY(path);
		}
		const char* name() const override { return "fs.model"; }

        // Replaces materials with default shader with given shader.
        void setMaterialsShader(std::shared_ptr<Shader> tShader, const bool tReplaceAll=false) {
            if(!hasModel()) return;
            for(uint m=0;m<model.materials.size();m++)
                if(model.materials[m].getShader()->getId()==Shader::getDefaultShader()->getId()||tReplaceAll) model.materials[m].setShader(tShader);
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Changed entity materials shader");
#endif // FS_PRINT_DEBUG_MSGS
        }
        // Replaces materials with shader id with given shader.
        void replaceMaterialsShader(const unsigned int& tIdToReplace, std::shared_ptr<Shader> tShader) {
            if(!hasModel()) return;
            for(uint m=0;m<model.materials.size();m++)
                if(model.materials[m].getShader()->getId()==tIdToReplace) model.materials[m].setShader(tShader);
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Replaced entity materials shader");
#endif // FS_PRINT_DEBUG_MSGS
        }
        // Replaces all materials with given one.
        void setMaterial(Material* tMaterial, const bool tReplaceAll=false) {
            if(!hasModel()) return;
            for(uint m=0;m<model.materials.size(); m++)
                if(model.materials[m].getShader()->getId()==Shader::getDefaultShader()->getId()||tReplaceAll) model.materials[m]=*tMaterial;
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Changed entity material");
#endif // FS_PRINT_DEBUG_MSGS
        }

        bool hasModel() const { return model.meshes.size()!=0; }
        virtual void removeModel(const bool& tRemoveMaterials=false) {
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Removed model from entity: \""+mEntity->name="\"");
#endif // FS_PRINT_DEBUG_MSGS
            for(uint i=0;i<model.meshes.size();i++)
                model.meshes[i].remove();
            model.nodes.clear();
            model.meshes.clear();
            if(tRemoveMaterials) model.materials.clear();
        }
        virtual bool load(const std::string& tPath) {
            if(!std::filesystem::exists(tPath)) {
                LOG_WARN("Model at path \"" + tPath + "\" doesn't exist");
                return false;
            }
            removeModel();
            LOG_INFO("Loading model at path \"" + tPath + "\"");
            
            auto extBig=String::split(tPath, '.');
            std::string ext=extBig[extBig.size()-1];
            if(ext.empty()) {LOG_ERRR("Looks like the extension of given model is invalid");}
#ifdef FS_LOADER_OBJ
            else if(ext=="obj") model=OBJ::load(tPath);
#endif // FS_LOADER_OBJ
#ifdef FS_LOADER_GLTF
            else if(ext=="gltf"||ext=="glb") model=GLTF::load(tPath,ext=="glb");
#endif // FS_LOADER_GLTF
#ifdef FS_LOADER_FBX
            else if(ext=="fbx") model=FBX::load(tPath);
#endif // FS_LOADER_FBX
            else LOG_ERRR("Looks like \"" + ext + " \" model format isn't supported. Please try obj, gltf, glb or fbx.");

            LOG_INFO("Loaded model at path \"" + tPath + "\"");
            return true;
        }
        virtual void addMesh(const Mesh& tMesh) {
#ifdef FS_PRINT_DEBUG_MSGS
            LOGF_DBG("Added custom mesh to entity with %d vertices and %d indicies",
                tMesh.vertices.size(), tMesh.indices.size());
#endif // FS_PRINT_DEBUG_MSGS
            ASSERT(tMesh.material,"There must be a material assigned to new mesh");
            model.meshes.emplace_back(tMesh);
            std::shared_ptr<Node> node=std::make_shared<Node>();
            node->name="Node_"+std::to_string(model.nodes.size());
            node->index=static_cast<int>(model.nodes.size());
            model.nodes.emplace_back(node);
        }
        virtual void addMesh(const std::vector<Vertex>& tVertices,
            const std::vector<unsigned int>& tIndices, Material* tMaterial) {
#ifdef FS_PRINT_DEBUG_MSGS
            LOGF_DBG("Added custom mesh to entity with %d vertices and %d indicies",
                tVertices.size(), tIndices.size());
#endif // FS_PRINT_DEBUG_MSGS
            ASSERT(tMaterial,"There must be a material assigned to new mesh");
            model.meshes.emplace_back(tVertices,tIndices,tMaterial);
            std::shared_ptr<Node> node=std::make_shared<Node>();
            node->name="Node_"+std::to_string(model.nodes.size());
            node->index=static_cast<int>(model.nodes.size());
            model.nodes.emplace_back(node);
        }

		std::string path;
        Model model;
	};
}

#endif // !FS_COMP_MODEL_H