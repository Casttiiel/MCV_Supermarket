#pragma once

class CTechnique;
class CTexture;
struct TCompBuffers;

class CMaterial : public IResource {

protected:
  static const int max_textures = 5;
  const CTexture*  textures[max_textures];
  bool             casts_shadows = true;
  const CMaterial* shadows_material = nullptr;

  CCteBuffer<TCtesMaterial> ctes_material;

  // To be able to upload all textures at once
  const ID3D11ShaderResourceView* srvs[max_textures];
  void cacheSRVs();

public:

  CMaterial();
  virtual ~CMaterial() {}

  virtual bool create(const json& j);
  virtual void destroy();
  virtual void activate() const;
  void activateTextures(int slot0) const;
  void activateCompBuffers(TCompBuffers* c_buffer) const;
  void renderInMenu() override;
  void onFileChanged(const std::string& filename);

  const CTechnique* tech = nullptr;
  eRenderCategory   category;
  int               priority = 100;

  const char* getCategoryName() const;
  bool castsShadows() const { return casts_shadows; }
  const CMaterial* getShadowsMaterial() const { return shadows_material; }
};

// --------------------------
using RenderCategoryNames = NamedValues<eRenderCategory>;
extern RenderCategoryNames category_names;