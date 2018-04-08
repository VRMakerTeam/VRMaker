#include <assert.h>
#include <QUuid>
#include "Utils.h"
#include "Composite.h"

QList<Composite::View*> Composite::Store::views_;


//-----------------------------------------------------------------------
Composite::Core::~Core()
{

}

//-----------------------------------------------------------------------
Composite::Data::~Data()
{

}

//-----------------------------------------------------------------------
bool Composite::View::HasTag(const QString& _tag)
{
	return tags_.contains(_tag);
}

//-----------------------------------------------------------------------
Composite::View::View()
{

}

//-----------------------------------------------------------------------
Composite::View::~View()
{

}
//-----------------------------------------------------------------------
void Composite::View::addChild(View* _view)
{
	if (children_.contains(_view))
		return;
	children_.append(_view);
}

//-----------------------------------------------------------------------
void Composite::View::removeChild(const QString& _uuid)
{
	QList<View*> views;
	for each (View* view in children_)
	{
		if (0 == view->componentUUID_.compare(_uuid))
			views.append(view);
	}

	for each (View* view in views)
	{
		children_.removeAll(view);
	}
}

//-----------------------------------------------------------------------
void Composite::View::addTag(const QString& _tag)
{
	if (tags_.contains(_tag))
		return;
	tags_.append(_tag);
}

//-----------------------------------------------------------------------
void Composite::View::removeTag(const QString& _tag)
{
	if (!tags_.contains(_tag))
		return;
	tags_.removeAll(_tag);
}

//-----------------------------------------------------------------------
void Composite::View::removeChild(View* _view)
{
	children_.removeAll(_view);
}

//-----------------------------------------------------------------------
void Composite::Store::bind(View* _view)
{
	if (views_.contains(_view))
		return;
	views_.append(_view);
}

void Composite::Store::unbind(View* _view)
{
	if (!views_.contains(_view))
		return;
	views_.removeAll(_view);
}

//-----------------------------------------------------------------------
void Composite::Store::Update(const QString& _tag)
{
	auto itr = views_.cbegin();
	for (; itr != views_.cend(); ++itr)
	{
		(*itr)->onUpdate(_tag);
	}
}

Composite::Component Composite::Component::root_;

//-----------------------------------------------------------------------
Composite::Component::Component()
{
	uuid_ = QUuid::createUuid().toString().remove("{").remove("}");
}

//-----------------------------------------------------------------------
Composite::Component::Component(const QString& _uuid)
{
	uuid_ = _uuid;
}

//-----------------------------------------------------------------------
Composite::Component::~Component()
{
}

//-----------------------------------------------------------------------
void Composite::Component::Initialize(Core* _core)
{
	initialize(_core);
	auto itr = children_.begin();
	for (; itr != children_.end(); ++itr)
	{
		(*itr)->Initialize(_core);
	}
}

//-----------------------------------------------------------------------
void Composite::Component::Release()
{
	auto itr = children_.begin();
	for (; itr != children_.end(); ++itr)
	{
		(*itr)->Release();
	}
	release();
}

//-----------------------------------------------------------------------
void Composite::Component::AddChild(Component* _component)
{
	assert(_component);
	assert(!(_component->GetUUID().isNull() || _component->GetUUID().isEmpty()));

	if (children_.contains(_component->GetUUID()))
		return;
	_component->parent_ = this;
	children_.insert(_component->GetUUID(), _component);
}

//-----------------------------------------------------------------------
void Composite::Component::RemoveChild(Component* _component)
{
	assert(_component);
	assert(!(_component->GetUUID().isNull() || _component->GetUUID().isEmpty()));

	if (!children_.contains(_component->GetUUID()))
		return;
	children_.remove(_component->GetUUID());
}

//-----------------------------------------------------------------------
void Composite::Component::RemoveChild(const QString& _uuid)
{
	if (!children_.contains(_uuid))
		return;
	children_.remove(_uuid);
}

//-----------------------------------------------------------------------
QString Composite::Component::GetUUID()
{
	return uuid_;
}

//-----------------------------------------------------------------------
Composite::Component* Composite::Component::FindChild(const QString& _uuid)
{
	if (!children_.contains(_uuid))
		return NULL;
	return children_.value(_uuid);
}

Composite::Component* Composite::Component::RecursiveFindChild(const QString& _uuid)
{
	Composite::Component* component = FindChild(_uuid);
	if (NULL == component)
	{
		auto itr = children_.begin();
		for (; itr != children_.end(); ++itr)
		{
			component = (*itr)->RecursiveFindChild(_uuid);
			if(NULL != component)
				break;
		}
	}
	return component;
}

//-----------------------------------------------------------------------
QList<QString> Composite::Component::ListChildren()
{
	return children_.keys();
}

//-----------------------------------------------------------------------
Composite::Component* Composite::Component::GetParent()
{
	return parent_;
}

//-----------------------------------------------------------------------
void Composite::Component::AddTag(const QString& _tag)
{
	if (tags_.contains(_tag))
		return;
	tags_.append(_tag);
}

//-----------------------------------------------------------------------
void Composite::Component::RemovevTag(const QString& _tag)
{
	if (!tags_.contains(_tag))
		return;
	tags_.removeAll(_tag);
}

//-----------------------------------------------------------------------
bool Composite::Component::HasTag(const QString& _tag)
{
	return tags_.contains(_tag);
}

//-----------------------------------------------------------------------
Composite::Component* Composite::Component::Root()
{
	return &root_;
}

//-----------------------------------------------------------------------
Composite::MVCComponent::MVCComponent(View* _view)
	: Composite::Component()
{
	view_ = _view;
	view_->componentUUID_ = uuid_;
	view_->setupUI();
	Composite::Store::bind(view_);
}

//-----------------------------------------------------------------------
Composite::MVCComponent::MVCComponent(View* _view, const QString& _uuid)
	: Composite::Component(_uuid)
{
	view_ = _view;
	view_->componentUUID_ = uuid_;
	view_->setupUI();
	Composite::Store::bind(view_);
}

//-----------------------------------------------------------------------
Composite::MVCComponent::~MVCComponent()
{
	Composite::Store::unbind(view_);
	SAFE_DELETE(view_);
}

//-----------------------------------------------------------------------
void Composite::MVCComponent::AddMVCChild(MVCComponent* _component)
{
	this->AddChild(_component);
	view_->addChild(_component->GetView());
	view_->onAttach(_component->GetView());
}
//-----------------------------------------------------------------------
void Composite::MVCComponent::RemoveMVCChild(MVCComponent* _component)
{
	this->RemoveChild(_component);
	view_->removeChild(_component->GetView());
}

void Composite::MVCComponent::RemoveMVCChild(const QString& _uuid)
{
	this->RemoveChild(_uuid);
	view_->removeChild(_uuid);
}

//-----------------------------------------------------------------------
Composite::View* Composite::MVCComponent::GetView()
{
	return view_;
}

//-----------------------------------------------------------------------
void Composite::MVCComponent::ResetUI(bool _recursive /* = false */)
{
	view_->reset();
	if (_recursive)
	{
		for each (QString uuid in this->ListChildren())
		{
			Composite::Component* component = this->FindChild(uuid);
			DYN_CAST(Composite::MVCComponent, component, source);
			source->ResetUI(_recursive);
		}
	}
}

//-----------------------------------------------------------------------
void Composite::MVCComponent::Refresh(bool _recursive /* = false */)
{
	refresh();
	if (_recursive)
	{
		for each (QString uuid in this->ListChildren())
		{
			Composite::Component* component = this->FindChild(uuid);
			DYN_CAST(Composite::MVCComponent, component, source);
			source->Refresh(_recursive);
		}
	}
}

void Composite::MVCComponent::AddTag(const QString& _tag)
{
	Component::AddTag(_tag);
	view_->addTag(_tag);
}

void Composite::MVCComponent::RemovevTag(const QString& _tag)
{
	Component::RemoveChild(_tag);
	view_->removeTag(_tag);
}


