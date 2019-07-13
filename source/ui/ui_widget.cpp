#pragma once

#include "mcv_platform.h"
#include "ui/ui_widget.h"
#include "ui/ui_effect.h"

namespace UI
{
  void CWidget::start()
  {
    for (auto fx : _effects)
    {
      fx->start();
    }

    for (auto& child : _children)
    {
      child->start();
    }
  }

  void CWidget::stop()
  {
    for (auto fx : _effects)
    {
      fx->stop();
    }

    for (auto& child : _children)
    {
      child->stop();
    }
  }

  void CWidget::update(float dt)
  {
    for (auto fx : _effects)
    {
      fx->update(dt);
    }

    for (auto& child : _children)
    {
      child->update(dt);
    }
  }

  void CWidget::doRender()
  {
    if (!_params.visible)
    {
      return;
    }

    render();

    for (auto& child : _children)
    {
      child->doRender();
    }
  }

  void CWidget::updateTransform()
  {
    computeAbsolute();

    for (auto& child : _children)
    {
      child->updateTransform();
    }
  }

  void CWidget::computePivot()
  {
    _pivot = MAT44::Identity * MAT44::CreateTranslation(-_params.pivot.x, -_params.pivot.y, 0.f);
  }

  void CWidget::computeLocal()
  {
    computePivot();

    MAT44 tr = MAT44::CreateTranslation(_params.position.x, _params.position.y, 0.f);
    MAT44 sc = MAT44::CreateScale(_params.scale.x, _params.scale.y, 0.f);
    MAT44 rot = MAT44::CreateRotationZ(_params.rotation);
    
    _local = rot * sc * tr;
  }

  void CWidget::computeAbsolute()
  {
    computeLocal();

    _absolute = _parent ? _local * _parent->_absolute : _local;
  }

  void CWidget::setParent(CWidget* parent)
  {
    removeFromParent();

    if (!parent)
    {
      return;
    }

    _parent = parent;
    _parent->_children.push_back(this);
  }

  void CWidget::removeFromParent()
  {
    if (!_parent)
    {
      return;
    }

    auto it = std::find(_parent->_children.begin(), _parent->_children.end(), this);
    assert(it != _parent->_children.end());
    _parent->_children.erase(it);
    _parent = nullptr;
  }
}
