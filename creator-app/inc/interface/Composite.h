#ifndef __COMPOSITE_H__
#define __COMPOSITE_H__

#include <QString>
#include <QHash>
#include <QList>

class QWidget;

namespace Composite
{
	class Core
	{
	protected:
		virtual ~Core();
	};

	class Data
	{
	protected:
		virtual ~Data();
	};

	class View
	{
		friend class Store;
		friend class MVCComponent;
	public:
		
		virtual void Show() {};
		virtual void Hide() {};

		template<typename T>
		T* FindChild()
		{
			for each (View* child in children_)
			{
				T* view = dynamic_cast<T*>(child);
				if (NULL != view)
					return view;
			}
			return NULL;
		}

		bool HasTag(const QString& _tag);
	protected:
		View();
		virtual ~View();
		virtual void setupUI() {};
		virtual void onUpdate(const QString& _tag) {};
		virtual void reset() {};
		virtual void onAttach(View* _view) {};
		QString componentUUID_;
		// key is uuid of component
		QList<View*> children_;
	private:
		void addChild(View* _view);
		void removeChild(View* _view);
		void removeChild(const QString& _uuid);
		void addTag(const QString& _tag);
		void removeTag(const QString& _tag);
		QStringList tags_;
	};


	class Store
	{
		friend class MVCComponent;
	public:
		static void Update(const QString& _tag);
	private:
		static void bind(View* _view);
		static void unbind(View* _view);
		static QList<View*> views_;
	};

	class Component
	{
		friend class MVCComponent;
	public:
		virtual ~Component();
		static Component* Root();

		//调用自己的initialize(), 再递归调用所有子组件的Initialize
		void Initialize(Core* _core);
		void Release();
		void AddChild(Component* _component);
		void RemoveChild(Component* _component);
		void RemoveChild(const QString& _uuid);
		QString GetUUID();
		Component* FindChild(const QString& _uuid);
		template<typename T>
		T* FindChild()
		{
			for each (Component* child in children_.values())
			{
				T* component = dynamic_cast<T*>(child);
				if (NULL != component)
					return component;
			}
			return NULL;
		}
		Component* RecursiveFindChild(const QString& _uuid);
		QList<QString> ListChildren();
		Component* GetParent();

		virtual void AddTag(const QString& _tag);
		virtual void RemovevTag(const QString& _tag);
		virtual bool HasTag(const QString& _tag);
	protected:
		Component();
		Component(const QString& _uuid);
		virtual void initialize(Core* _core) {};
		virtual void release() {};
		View* view_;
		QStringList tags_;
	private:
		Component* parent_;
		QHash<QString, Component*> children_;
		QString uuid_;
		static Component root_;
	};

	class MVCComponent : public Component
	{
	public:
		void AddMVCChild(MVCComponent* _component);
		void RemoveMVCChild(MVCComponent* _component);
		void RemoveMVCChild(const QString& _uuid);

		View* GetView();

		void ResetUI(bool _recursive = false);
		void Refresh(bool _recursive = false);

		void AddTag(const QString& _tag) override;
		void RemovevTag(const QString& _tag) override;
	protected:
		/// \brief 同时调用view.setupUI
		MVCComponent(View* _view);
		MVCComponent(View* _view, const QString& _uuid);
		virtual ~MVCComponent();
		virtual void refresh() {};
		View* view_;
	private:
	};
}

#endif
