#pragma once

#include "mcv_platform.h"
#include "ui/ui_params.h"

namespace UI
{
  class CEffect;

  class CWidget
  {
  public:
    const std::string& getName() const { return _name; }
    const std::string& getAlias() const { return _alias; }
    void doRender();
    void start();
    void stop();

    virtual void update(float dt);
    virtual void render() {}

    virtual TParams* getParams() { return &_params; }
    virtual TImageParams* getImageParams() { return nullptr; }

    void updateTransform();
    void setParent(CWidget* parent);
    void removeFromParent();

	MAT44 getAbsolute() { return _absolute; };

  protected:
    void computePivot();
    void computeLocal();
    void computeAbsolute();

    std::string _name;
    std::string _alias;
    TParams _params;
    MAT44 _local;
    MAT44 _pivot;
    MAT44 _absolute;
    CWidget* _parent = nullptr;
    std::vector<CWidget*> _children;
    std::vector<CEffect*> _effects;

    friend class CParser;
  };
}
