using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.EventSystems;
using System.Linq;

using UnityEngine;
using UnityEngine.UI;

namespace Battlehub.UIControls
{
    /// <summary>
    /// Selection Change Event Arguments
    /// </summary>
    public class SelectionChangedEventArgs : EventArgs
    {
        /// <summary>
        /// Unselected Items
        /// </summary>
        public object[] OldItems
        {
            get;
            private set;
        }

        /// <summary>
        /// Selected Items
        /// </summary>
        public object[] NewItems
        {
            get;
            private set;
        }

        /// <summary>
        /// First Unselected Item
        /// </summary>
        public object OldItem
        {
            get
            {
                if (OldItems == null)
                {
                    return null;
                }
                return OldItems[0];
            }
        }

        /// <summary>
        /// First Selected Item
        /// </summary>
        public object NewItem
        {
            get
            {
                if (NewItems == null)
                {
                    return null;
                }
                return NewItems[0];
            }
        }

        public SelectionChangedEventArgs(object[] oldItems, object[] newItems)
        {
            OldItems = oldItems;
            NewItems = newItems;
        }

        public SelectionChangedEventArgs(object oldItem, object newItem)
        {
            OldItems = new[] { oldItem };
            NewItems = new[] { newItem };
        }
    }
    
    /// <summary>
    /// Items Removed Arguments
    /// </summary>
    public class ItemsRemovedArgs : EventArgs
    {
        /// <summary>
        /// Removed Items
        /// </summary>
        public object[] Items
        {
            get;
            private set;
        }

        public ItemsRemovedArgs(object[] items)
        {
            Items = items;
        }
    }

    /// <summary>
    /// Item DataBinding Args
    /// </summary>
    public class ItemDataBindingArgs : EventArgs
    {
        /// <summary>
        /// Data Item 
        /// </summary>
        public object Item
        {
            get;
            set;
        }

        /// <summary>
        /// UI Presenter For Data Item
        /// </summary>
        public GameObject ItemPresenter
        {
            get;
            set;
        }
    }

    /// <summary>
    /// Item Drag Arguments
    /// </summary>
    public class ItemDragArgs : EventArgs
    {
        /// <summary>
        /// Data Item being dragged
        /// </summary>
        public object DragItem
        {
            get;
            private set;
        }

        public ItemDragArgs(object[] dragItem)
        {
            DragItem = dragItem;
        }
    }

    /// <summary>
    /// Item Drop Arguments
    /// </summary>
    public class ItemDropArgs : EventArgs
    {
        /// <summary>
        /// Data Items which are going to be dropped to new location
        /// </summary>
        public object[] DragItems
        {
            get;
            private set;
        }

        /// <summary>
        /// Drop Target
        /// </summary>
        public object DropTarget
        {
            get;
            private set;
        }

        /// <summary>
        /// Set Last Child - DropTarget will be considered as parent
        /// Set Next Sibling - DropTarget will be considered as prev sibling
        /// Set Prev Sibling - DropTarget will be considered as next sibling
        /// </summary>
        public ItemDropAction Action
        {
            get;
            private set;
        }

        /// <summary>
        /// True if DragItems belongs to other control
        /// </summary>
        public bool IsExternal
        {
            get;
            private set;
        }

        public ItemDropArgs(object[] dragItems, object dropTarget, ItemDropAction action, bool isExternal)
        {
            DragItems = dragItems;
            DropTarget = dropTarget;
            Action = action;
            IsExternal = isExternal;
        }
    }

    /// <summary>
    /// Generic ItemsControl
    /// </summary>
    public class ItemsControl : ItemsControl<ItemDataBindingArgs>
    {
    }

    /// <summary>
    /// Base class for item controls
    /// </summary>
    /// <typeparam name="TDataBindingArgs">Data Binding Event Arguments</typeparam>
    public class ItemsControl<TDataBindingArgs> : MonoBehaviour, IPointerDownHandler, IDropHandler where TDataBindingArgs : ItemDataBindingArgs, new()
    {
        /// <summary>
        /// Raised on begin drag
        /// </summary>
        public event EventHandler<ItemDragArgs> ItemBeginDrag;
        /// <summary>
        /// Raised on item dropped
        /// </summary>
        public event EventHandler<ItemDropArgs> ItemDrop;
        /// <summary>
        /// Raised when drag operation completed
        /// </summary>
        public event EventHandler<ItemDragArgs> ItemEndDrag;
        /// <summary>
        /// Raised when item data binding required
        /// </summary>
        public event EventHandler<TDataBindingArgs> ItemDataBinding;
        /// <summary>
        /// Raised on selection changed
        /// </summary>
        public event EventHandler<SelectionChangedEventArgs> SelectionChanged;
        /// <summary>
        /// Raised when item removed
        /// </summary>
        public event EventHandler<ItemsRemovedArgs> ItemsRemoved;

        /// <summary>
        /// Multiselect operation key
        /// </summary>
        public KeyCode MultiselectKey = KeyCode.LeftControl;

        /// <summary>
        /// Rangeselect operation key
        /// </summary>
        public KeyCode RangeselectKey = KeyCode.LeftShift;

        /// <summary>
        /// Remove key 
        /// </summary>
        public KeyCode RemoveKey = KeyCode.Delete;
        private bool m_prevCanDrag;
        public bool CanDrag = true;

        private bool m_isDropInProgress;
        /// <summary>
        /// Protection against stack overflow
        /// </summary>
        protected bool IsDropInProgress
        {
            get { return m_isDropInProgress; }
        }

        /// <summary>
        /// Pefab for data item presentation
        /// </summary>
        [SerializeField]
        private GameObject ItemContainerPrefab;

        /// <summary>
        /// Items Panel
        /// </summary>
        [SerializeField]
        private Transform Panel;

        /// <summary>
        /// Root canvas
        /// </summary>
        private Canvas m_canvas;

        /// <summary>
        /// Raycasting camera (used with world space canvas)
        /// </summary>
        public Camera Camera;

        /// <summary>
        /// ScrollView scroll speed during drag drop operaiton
        /// </summary>
        public float ScrollSpeed = 100;
        private enum ScrollDir
        {
            None,
            Up,
            Down,
            Left,
            Right
        }
        private ScrollDir m_scrollDir;
        private ScrollRect m_scrollRect;
        /// <summary>
        /// ScrollView RectTransformChange listener
        /// </summary>
        private RectTransformChangeListener m_rtcListener;
        
        /// <summary>
        /// Viewport effective width
        /// </summary>
        private float m_width;

        /// <summary>
        /// list of ItemContainers
        /// </summary>
        private List<ItemContainer> m_itemContainers;

        /// <summary>
        /// marker used to display item drop location
        /// </summary>
        private ItemDropMarker m_dropMarker;

        /// <summary>
        /// drop target
        /// </summary>
        private ItemContainer m_dropTarget;

        /// <summary>
        /// drag items
        /// </summary>
        private ItemContainer[] m_dragItems;

        protected ItemDropMarker DropMarker
        {
            get { return m_dropMarker; }
        }

        private IList<object> m_items;
        /// <summary>
        /// Data Items
        /// </summary>
        public IEnumerable Items
        {
            get { return m_items; }
            set
            {
                m_items = value.OfType<object>().ToList();
                DataBind();
            }
        }

        /// <summary>
        /// Count of dataItems
        /// </summary>
        public int ItemsCount
        {
            get
            {
                if (m_items == null)
                {
                    return 0;
                }

                return m_items.Count;
            }
        }

        protected void RemoveItemAt(int index)
        {
            m_items.RemoveAt(index);
        }

        protected void RemoveItemContainerAt(int index)
        {
            m_itemContainers.RemoveAt(index);
        }

        protected void InsertItem(int index, object value)
        {
            m_items.Insert(index, value);
        }

        protected void InsertItemContainerAt(int index, ItemContainer container)
        {
            m_itemContainers.Insert(index, container);
        }

        /// <summary>
        /// Selected Items Count
        /// </summary>
        public int SelectedItemsCount
        {
            get
            {
                if (m_selectedItems == null)
                {
                    return 0;
                }

                return m_selectedItems.Count;
            }
        }

        /// <summary>
        /// SelectedItems property setter is not reenterant
        /// </summary>
        private bool m_selectionLocked;
        private List<object> m_selectedItems;
        /// <summary>
        /// Selected DataItems
        /// </summary>
        public IEnumerable SelectedItems
        {
            get { return m_selectedItems; }
            set
            {
                if (m_selectionLocked)
                {
                    return;
                }
                m_selectionLocked = true;

                IList oldSelectedItems = m_selectedItems;
                if (value != null)
                {
                    m_selectedItems = value.OfType<object>().ToList();
                    for (int i = m_selectedItems.Count - 1; i >= 0; --i)
                    {
                        object item = m_selectedItems[i];
                        ItemContainer container = GetItemContainer(item);
                        if (container == null)
                        {
                            m_selectedItems.Remove(item);
                        }
                        else
                        {
                            container.IsSelected = true;
                        }
                    }
                    if (m_selectedItems.Count == 0)
                    {
                        m_selectedItem = null;
                        m_selectedIndex = -1;
                    }
                    else
                    {
                        m_selectedItem = GetItemContainer(m_selectedItems[0]);
                        m_selectedIndex = IndexOf(m_selectedItem.Item);
                    }
                }
                else
                {
                    m_selectedItems = null;
                    m_selectedItem = null;
                    m_selectedIndex = -1;
                }

                List<object> unselectedItems = new List<object>();
                if (oldSelectedItems != null)
                {
                    if (m_selectedItems != null)
                    {
                        for (int i = 0; i < oldSelectedItems.Count; ++i)
                        {
                            object oldItem = oldSelectedItems[i];
                            if (!m_selectedItems.Contains(oldItem))
                            {
                                unselectedItems.Add(oldItem);
                                ItemContainer container = GetItemContainer(oldItem);
                                container.IsSelected = false;
                            }
                        }
                    }
                    else
                    {
                        unselectedItems.AddRange(oldSelectedItems.OfType<object>());
                    }
                }

                if (SelectionChanged != null)
                {
                    object[] selectedItems = m_selectedItems == null ? new object[0] : m_selectedItems.ToArray();
                    SelectionChanged(this, new SelectionChangedEventArgs(unselectedItems.ToArray(), selectedItems));
                }

                m_selectionLocked = false;
            }
        }

        private ItemContainer m_selectedItem;
        /// <summary>
        /// First selecte item (null if no items selected)
        /// </summary>
        public object SelectedItem
        {
            get
            {
                if (m_selectedItem == null)
                {
                    return null;
                }
                return m_selectedItem.Item;
            }
            set
            {
                SelectedIndex = IndexOf(value);
            }
        }

        private int m_selectedIndex = -1;
        /// <summary>
        /// First selected item index (-1 if no items selected)
        /// </summary>
        public int SelectedIndex
        {
            get
            {
                if (m_selectedItem == null)
                {
                    return -1;
                }

                return m_selectedIndex;
            }
            set
            {
                if (m_selectedIndex == value)
                {
                    return;
                }

                ItemContainer oldItemContainer = m_selectedItem;
                if (oldItemContainer != null)
                {
                    oldItemContainer.IsSelected = false;
                }

                m_selectedIndex = value;
                object newItem = null;
                if (m_selectedIndex >= 0 && m_selectedIndex < m_items.Count)
                {
                    newItem = m_items[m_selectedIndex];
                    m_selectedItem = GetItemContainer(newItem);

                    if (m_selectedItem != null)
                    {
                        m_selectedItem.IsSelected = true;
                    }
                }

                object[] newItems = newItem != null ? new[] { newItem } : new object[0];
                object[] oldItems = m_selectedItems == null ? new object[0] : m_selectedItems.Except(newItems).ToArray();
                for (int i = 0; i < oldItems.Length; ++i)
                {
                    object oldItem = oldItems[i];
                    ItemContainer container = GetItemContainer(oldItem);
                    container.IsSelected = false;
                }

                m_selectedItems = newItems.ToList();


                if (SelectionChanged != null)
                {
                    SelectionChanged(this, new SelectionChangedEventArgs(oldItems, newItems));
                }
            }
        }

        /// <summary>
        /// Index of dataitem
        /// </summary>
        /// <param name="obj"></param>
        /// <returns></returns>
        public int IndexOf(object obj)
        {
            if (m_items == null)
            {
                return -1;
            }

            if (obj == null)
            {
                return -1;
            }

            return m_items.IndexOf(obj);
        }

        /// <summary>
        /// Get ItemContainer for data item
        /// </summary>
        /// <param name="obj">data item</param>
        /// <returns>data item or null if item not found</returns>
        public ItemContainer GetItemContainer(object obj)
        {
            return m_itemContainers.Where(ic => ic.Item == obj).FirstOrDefault();
        }

        /// <summary>
        /// Get Last Item Container
        /// </summary>
        /// <returns>last item container</returns>
        public ItemContainer LastItemContainer()
        {
            if (m_itemContainers == null || m_itemContainers.Count == 0)
            {
                return null;
            }

            return m_itemContainers[m_itemContainers.Count - 1];
        }

        /// <summary>
        /// Get Item Container at index
        /// </summary>
        /// <param name="siblingIndex">item container index</param>
        /// <returns>ItemContainer or null if index out of range</returns>
        public ItemContainer GetItemContainer(int siblingIndex)
        {
            if (siblingIndex < 0 || siblingIndex >= m_itemContainers.Count)
            {
                return null;
            }

            return m_itemContainers[siblingIndex];
        }

        /// <summary>
        /// Add item to the end (NOTE: if you add more than one item consider using Items property)
        /// </summary>
        /// <param name="item">data item to add</param>
        /// <returns>ItemContainer for data item</returns>
        public ItemContainer Add(object item)
        {
            if (m_items == null)
            {
                m_items = new List<object>();
                m_itemContainers = new List<ItemContainer>();
            }
            return Insert(m_items.Count, item);
        }

        /// <summary>
        /// Insert data item (NOTE: if you insert more than one item consider using Items property)
        /// </summary>
        /// <param name="index">index</param>
        /// <param name="item">data item</param>
        /// <returns>Item container for data item</returns>
        public ItemContainer Insert(int index, object item)
        {
            if (m_items == null)
            {
                m_items = new List<object>();
                m_itemContainers = new List<ItemContainer>();
            }
            object itemAtIndex = m_items.ElementAtOrDefault(index);
            ItemContainer itemContainer = GetItemContainer(itemAtIndex);

            int itemContainerIndex;
            if (itemContainer != null)
            {
                itemContainerIndex = m_itemContainers.IndexOf(itemContainer);
            }
            else
            {
                itemContainerIndex = m_itemContainers.Count;
            }

            m_items.Insert(index, item);
            itemContainer = InstantiateItemContainer(itemContainerIndex);
            if (itemContainer != null)
            {
                itemContainer.Item = item;
                DataBindItem(item, itemContainer);
            }

            return itemContainer;
        }

        /// <summary>
        /// Remove data item
        /// </summary>
        /// <param name="item">data item to remove</param>
        public void Remove(object item)
        {
            if (item == null)
            {
                return;
            }

            if (m_items == null)
            {
                return;
            }

            if (!m_items.Contains(item))
            {
                return;
            }

            DestroyItem(item);
        }

        /// <summary>
        /// Remove data item at index
        /// </summary>
        /// <param name="index">index</param>
        public void RemoveAt(int index)
        {
            if (m_items == null)
            {
                return;
            }

            if (index >= m_items.Count || index < 0)
            {
                throw new ArgumentOutOfRangeException("index");
            }

            Remove(m_items[index]);
        }

        /// <summary>
        /// Lifecycle methods
        /// </summary>
        private void Awake()
        {
            if (Panel == null)
            {
                Panel = transform;
            }

            m_itemContainers = GetComponentsInChildren<ItemContainer>().ToList();
            m_rtcListener = GetComponentInChildren<RectTransformChangeListener>();
            if(m_rtcListener != null)
            {
                m_rtcListener.RectTransformChanged += OnViewportRectTransformChanged;
            }
            m_dropMarker = GetComponentInChildren<ItemDropMarker>(true);
            m_scrollRect = GetComponent<ScrollRect>();
            
            if (Camera == null)
            {
                Camera = Camera.main;
            }

            m_prevCanDrag = CanDrag;
            OnCanDragChanged();

            AwakeOverride();
        }

        private void Start()
        {
            m_canvas = GetComponentInParent<Canvas>();
            StartOverride();
        }

        private void Update()
        {
            if (m_scrollDir != ScrollDir.None)
            {
                float verDelta = (m_scrollRect.content.rect.height - m_scrollRect.viewport.rect.height);
                float verOffset = 0;
                if (verDelta > 0)
                {
                    verOffset = (ScrollSpeed / 10.0f) * (1.0f / verDelta);
                }

                float horDelta = (m_scrollRect.content.rect.width - m_scrollRect.viewport.rect.width);
                float horOffset = 0;
                if (horDelta > 0)
                {
                    horOffset = (ScrollSpeed / 10.0f) * (1.0f / horDelta);
                }

                if (m_scrollDir == ScrollDir.Up)
                {
                    m_scrollRect.verticalNormalizedPosition += verOffset;
                    if (m_scrollRect.verticalNormalizedPosition > 1)
                    {
                        m_scrollRect.verticalNormalizedPosition = 1;
                        m_scrollDir = ScrollDir.None;
                    }
                }
                else if (m_scrollDir == ScrollDir.Down)
                {
                    m_scrollRect.verticalNormalizedPosition -= verOffset;
                    if (m_scrollRect.verticalNormalizedPosition < 0)
                    {
                        m_scrollRect.verticalNormalizedPosition = 0;
                        m_scrollDir = ScrollDir.None;
                    }
                }
                else if (m_scrollDir == ScrollDir.Left)
                {
                    m_scrollRect.horizontalNormalizedPosition -= horOffset;
                    if (m_scrollRect.horizontalNormalizedPosition < 0)
                    {
                        m_scrollRect.horizontalNormalizedPosition = 0;
                        m_scrollDir = ScrollDir.None;
                    }
                }
                if (m_scrollDir == ScrollDir.Right)
                {

                    m_scrollRect.horizontalNormalizedPosition += horOffset;
                    if (m_scrollRect.horizontalNormalizedPosition > 1)
                    {
                        m_scrollRect.horizontalNormalizedPosition = 1;
                        m_scrollDir = ScrollDir.None;
                    }
                }
            }

            if (Input.GetKeyDown(RemoveKey))
            {
                DestroySelectedItems();
            }

            if (m_prevCanDrag != CanDrag)
            {
                OnCanDragChanged();
                m_prevCanDrag = CanDrag;
            }

            UpdateOverride();
        }

        private void OnEnable()
        {
            ItemContainer.Selected += OnItemSelected;
            ItemContainer.Unselected += OnItemUnselected;
            ItemContainer.PointerUp += OnItemPointerUp;
            ItemContainer.PointerDown += OnItemPointerDown;
            ItemContainer.PointerEnter += OnItemPointerEnter;
            ItemContainer.PointerExit += OnItemPointerExit;
            ItemContainer.BeginDrag += OnItemBeginDrag;
            ItemContainer.Drag += OnItemDrag;
            ItemContainer.Drop += OnItemDrop;
            ItemContainer.EndDrag += OnItemEndDrag;

            OnEnableOverride();
        }

        private void OnDisable()
        {
            ItemContainer.Selected -= OnItemSelected;
            ItemContainer.Unselected -= OnItemUnselected;
            ItemContainer.PointerUp -= OnItemPointerUp;
            ItemContainer.PointerDown -= OnItemPointerDown;
            ItemContainer.PointerEnter -= OnItemPointerEnter;
            ItemContainer.PointerExit -= OnItemPointerExit;
            ItemContainer.BeginDrag -= OnItemBeginDrag;
            ItemContainer.Drag -= OnItemDrag;
            ItemContainer.Drop -= OnItemDrop;
            ItemContainer.EndDrag -= OnItemEndDrag;

            OnDisableOverride();
        }

        private void OnDestroy()
        {
            if (m_rtcListener != null)
            {
                m_rtcListener.RectTransformChanged -= OnViewportRectTransformChanged;
            }
            OnDestroyOverride();
        }

        /// <summary>
        /// Lifecycle method overrides
        /// </summary>
        protected virtual void AwakeOverride()
        {

        }

        protected virtual void StartOverride()
        {

        }

        protected virtual void UpdateOverride()
        {

        }

        protected virtual void OnEnableOverride()
        {

        }

        protected virtual void OnDisableOverride()
        {

        }

        protected virtual void OnDestroyOverride()
        {

        }

        /// <summary>
        /// Viewport transform change event handler
        /// </summary>
        private void OnViewportRectTransformChanged()
        {
            if (m_scrollRect.viewport.rect.width != m_width)
            {
                m_width = m_scrollRect.viewport.rect.width;
                if (m_itemContainers != null)
                {
                    for (int i = 0; i < m_itemContainers.Count; ++i)
                    {
                        ItemContainer container = m_itemContainers[i];
                        if (container != null)
                        {
                            container.LayoutElement.minWidth = m_width;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Update all data containers with ItemsControl CanDrag field value
        /// </summary>
        private void OnCanDragChanged()
        {
            for (int i = 0; i < m_itemContainers.Count; ++i)
            {
                ItemContainer container = m_itemContainers[i];
                if (container != null)
                {
                    container.CanDrag = CanDrag;
                }
            }
        }

        /// <summary>
        /// Whether event can be handled by ItemsControl
        /// </summary>
        /// <param name="sender"></param>
        /// <returns></returns>
        protected bool CanHandleEvent(object sender)
        {
            ItemContainer itemContainer = sender as ItemContainer;
            if (!itemContainer)
            {
                return false;
            }
            return itemContainer.transform.IsChildOf(Panel);
        }

        /// <summary>
        /// Called when item selected
        /// </summary>
        private void OnItemSelected(object sender, EventArgs e)
        {
            if (m_selectionLocked)
            {
                return;
            }

            if (!CanHandleEvent(sender))
            {
                return;
            }

            if (Input.GetKey(MultiselectKey))
            {
                IList selectedItems = m_selectedItems != null ? m_selectedItems.ToList() : new List<object>();
                selectedItems.Add(((ItemContainer)sender).Item);
                SelectedItems = selectedItems;
            }
            else if (Input.GetKey(RangeselectKey))
            {
                SelectRange((ItemContainer)sender);
            }
            else
            {
                SelectedIndex = IndexOf(((ItemContainer)sender).Item);
            }
        }

        /// <summary>
        /// Select Data Items Range between current selection and itemContainer inclusive
        /// </summary>
        /// <param name="itemContainer">last item container to be included in range selection</param>
        private void SelectRange(ItemContainer itemContainer)
        {
            if (m_selectedItems != null && m_selectedItems.Count > 0)
            {
                List<object> selectedItems = new List<object>();
                int firstItemIndex = IndexOf(m_selectedItems[0]);

                object item = itemContainer.Item;
                int lastItemIndex = IndexOf(item);

                int minIndex = Mathf.Min(firstItemIndex, lastItemIndex);
                int maxIndex = Math.Max(firstItemIndex, lastItemIndex);

                selectedItems.Add(m_selectedItems[0]);
                for (int i = minIndex; i < firstItemIndex; ++i)
                {
                    selectedItems.Add(m_items[i]);
                }
                for (int i = firstItemIndex + 1; i <= maxIndex; ++i)
                {
                    selectedItems.Add(m_items[i]);
                }
                SelectedItems = selectedItems;

            }
            else
            {
                SelectedIndex = IndexOf(itemContainer.Item);
            }
        }

        /// <summary>
        /// Called when item unselected
        /// </summary>
        private void OnItemUnselected(object sender, EventArgs e)
        {
            if (m_selectionLocked)
            {
                return;
            }

            if (!CanHandleEvent(sender))
            {
                return;
            }

            IList selectedItems = m_selectedItems != null ? m_selectedItems.ToList() : new List<object>();
            selectedItems.Remove(((ItemContainer)sender).Item);
            SelectedItems = selectedItems;
        }

        /// <summary>
        /// Called when item get pointer down event. Primarily used to handle selection
        /// </summary>
        private void OnItemPointerDown(ItemContainer sender, PointerEventData e)
        {
            if (!CanHandleEvent(sender))
            {
                return;
            }

            m_dropMarker.SetTraget(null);
            m_dragItems = null;
            m_isDropInProgress = false;

            if (Input.GetKey(RangeselectKey))
            {
                SelectRange(sender);
            }
            else if (Input.GetKey(MultiselectKey))
            {
                sender.IsSelected = !sender.IsSelected;
            }
            else
            {
                sender.IsSelected = true;
            }
        }

        /// <summary>
        /// Called when item get pointer up event. Primarily used to handle selection
        /// </summary>
        private void OnItemPointerUp(ItemContainer sender, PointerEventData e)
        {
            if (!CanHandleEvent(sender))
            {
                return;
            }

            if (m_dragItems != null)
            {
                return;
            }

            if (!Input.GetKey(MultiselectKey) && !Input.GetKey(RangeselectKey))
            {
                if (m_selectedItems != null && m_selectedItems.Count > 1)
                {
                    if(SelectedItem == sender.Item)
                    {
                        SelectedItem = null;
                    }
                    SelectedItem = sender.Item;
                }
            }
        }

        /// <summary>
        /// Used during drag drop operation
        /// </summary>
        private void OnItemPointerEnter(ItemContainer sender, PointerEventData eventData)
        {
            if (!CanHandleEvent(sender))
            {
                return;
            }
            m_dropTarget = sender;
            if (m_dragItems != null)
            {
                if (m_scrollDir == ScrollDir.None)
                {
                    m_dropMarker.SetTraget(m_dropTarget);
                }
            }
        }

        /// <summary>
        /// Used during drag drop operation
        /// </summary>
        private void OnItemPointerExit(ItemContainer sender, PointerEventData eventData)
        {
            if (!CanHandleEvent(sender))
            {
                return;
            }
            m_dropTarget = null;
            if (m_dragItems != null)
            {
                m_dropMarker.SetTraget(null);
            }
        }

        /// <summary>
        /// Called on the beginning of drag operation
        /// </summary>
        private void OnItemBeginDrag(ItemContainer sender, PointerEventData eventData)
        {
            eventData.Reset();
            if (!CanHandleEvent(sender))
            {
                return;
            }

            if (m_dropTarget != null)
            {
                m_dropMarker.SetTraget(m_dropTarget);
                m_dropMarker.SetPosition(eventData.position);
            }

            m_dragItems = GetDragItems();
            if (ItemBeginDrag != null)
            {
                ItemBeginDrag(this, new ItemDragArgs(m_dragItems.Select(di => di.Item).ToArray()));
            }
        }

        /// <summary>
        /// Called when item being dragged
        /// </summary>
        private void OnItemDrag(ItemContainer sender, PointerEventData eventData)
        {
            if (!CanHandleEvent(sender))
            {
                return;
            }
            if (m_dropTarget != null)
            {
                m_dropMarker.SetPosition(eventData.position);
            }

            float viewportHeight = m_scrollRect.viewport.rect.height;
            float viewportWidth = m_scrollRect.viewport.rect.width;

            Camera camera = null;
            if (m_canvas.renderMode == RenderMode.WorldSpace)
            {
                camera = Camera;
            }

            Vector2 localPoint;
            if (RectTransformUtility.ScreenPointToLocalPointInRectangle(m_scrollRect.viewport, eventData.position, camera, out localPoint))
            {
                if (localPoint.y >= 0)
                {
                    m_scrollDir = ScrollDir.Up;
                    m_dropMarker.SetTraget(null);
                }
                else if (localPoint.y < -viewportHeight)
                {
                    m_scrollDir = ScrollDir.Down;
                    m_dropMarker.SetTraget(null);
                }
                else if (localPoint.x <= 0)
                {
                    m_scrollDir = ScrollDir.Left;
                }
                else if (localPoint.x >= viewportWidth)
                {
                    m_scrollDir = ScrollDir.Right;
                }
                else
                {
                    m_scrollDir = ScrollDir.None;
                }
            }
        }

        /// <summary>
        /// Called on item dropped
        /// </summary>
        private void OnItemDrop(ItemContainer sender, PointerEventData eventData)
        {
            if (!CanHandleEvent(sender))
            {
                return;
            }

            m_isDropInProgress = true; //Prevent ChangeParent operation
            try
            {
                if (CanDrop(m_dragItems, m_dropTarget))
                {
                    Drop(m_dragItems, m_dropTarget, m_dropMarker.Action);
                    if (ItemDrop != null)
                    {
                        if(m_dragItems == null)
                        {
                            Debug.LogError("m_dragItems");
                        }
                        if (m_dropTarget == null)
                        {
                            Debug.LogError("m_dropTarget");
                        }
                        if (m_dropMarker == null)
                        {
                            Debug.LogError("m_dropMarker");
                        }
                        ItemDrop(this, new ItemDropArgs(m_dragItems.Select(di => di.Item).ToArray(), m_dropTarget.Item, m_dropMarker.Action, false));
                    }
                }
                RaiseEndDrag();
            }
            finally
            {
                m_isDropInProgress = false;
            }
        }

        private void OnItemEndDrag(ItemContainer sender, PointerEventData eventData)
        {
            if (!CanHandleEvent(sender))
            {
                return;
            }
            RaiseEndDrag();
        }

        private void RaiseEndDrag()
        {
            if (m_dragItems != null)
            {
                if (ItemEndDrag != null)
                {
                    ItemEndDrag(this, new ItemDragArgs(m_dragItems.Select(di => di.Item).ToArray()));
                }
                m_dropMarker.SetTraget(null);
                m_dragItems = null;
                m_scrollDir = ScrollDir.None;
            }
        }

        void IDropHandler.OnDrop(PointerEventData eventData)
        {
            if (m_dragItems == null)
            {
                GameObject go = eventData.pointerDrag;
                if (go != null)
                {
                    ItemContainer itemContainer = go.GetComponent<ItemContainer>();
                    if (itemContainer != null && itemContainer.Item != null)
                    {
                        object item = itemContainer.Item;
                        if (ItemDrop != null)
                        {
                            ItemDrop(this, new ItemDropArgs(new[] { item }, null, ItemDropAction.SetLastChild, true));
                        }
                    }
                }
                return;
            }

            if (m_itemContainers != null && m_itemContainers.Count > 0)
            {
                m_dropTarget = m_itemContainers.Last();
                m_dropMarker.Action = ItemDropAction.SetNextSibling;
            }

            m_isDropInProgress = true; //Prevent ChangeParent operation
            try
            {
                if (CanDrop(m_dragItems, m_dropTarget))
                {

                    if (ItemDrop != null)
                    {
                        ItemDrop(this, new ItemDropArgs(m_dragItems.Select(di => di.Item).ToArray(), m_dropTarget.Item, m_dropMarker.Action, false));
                    }
                    Drop(m_dragItems, m_dropTarget, m_dropMarker.Action);
                }

                m_dropMarker.SetTraget(null);
                m_dragItems = null;
            }
            finally
            {
                m_isDropInProgress = false;
            }
        }

        void IPointerDownHandler.OnPointerDown(PointerEventData eventData)
        {
            SelectedIndex = -1;
        }

        protected virtual bool CanDrop(ItemContainer[] dragItems, ItemContainer dropTarget)
        {
            if (dropTarget == null)
            {
                return true;
            }

            if (dragItems == null)
            {
                return false;
            }

            if (dragItems.Contains(dropTarget.Item))
            {
                return false;
            }

            return true;
        }

        protected ItemContainer[] GetDragItems()
        {
            ItemContainer[] dragItems = new ItemContainer[m_selectedItems.Count];
            if (m_selectedItems != null)
            {
                for (int i = 0; i < m_selectedItems.Count; ++i)
                {
                    dragItems[i] = GetItemContainer(m_selectedItems[i]);
                }
            }

            return dragItems.OrderBy(di => di.transform.GetSiblingIndex()).ToArray();
        }

        protected virtual void DropItemAfter(ItemContainer dropTarget, ItemContainer dragItem)
        {
            int dropContainerIndex = dropTarget.transform.GetSiblingIndex();
            int dragContainerIndex = dragItem.transform.GetSiblingIndex();
            RemoveItemContainerAt(dragContainerIndex);
            RemoveItemAt(dragContainerIndex);

            if (dragContainerIndex > dropContainerIndex)
            {
                dropContainerIndex++;
            }

            dragItem.transform.SetSiblingIndex(dropContainerIndex);
            InsertItemContainerAt(dropContainerIndex, dragItem);
            InsertItem(dropContainerIndex, dragItem.Item);

        }

        protected virtual void DropItemBefore(ItemContainer dropTarget, ItemContainer dragItem)
        {
            int dropContainerIndex = dropTarget.transform.GetSiblingIndex();
            int dragContainerIndex = dragItem.transform.GetSiblingIndex();
            RemoveItemContainerAt(dragContainerIndex);
            RemoveItemAt(dragContainerIndex);

            if (dragContainerIndex < dropContainerIndex)
            {
                dropContainerIndex--;
            }

            dragItem.transform.SetSiblingIndex(dropContainerIndex);
            InsertItemContainerAt(dropContainerIndex, dragItem);
            InsertItem(dropContainerIndex, dragItem.Item);
        }

        protected virtual void Drop(ItemContainer[] dragItems, ItemContainer dropTarget, ItemDropAction action)
        {
            if (action == ItemDropAction.SetPrevSibling)
            {
                for (int i = 0; i < dragItems.Length; ++i)
                {
                    ItemContainer dragItem = dragItems[i];
                    DropItemBefore(dropTarget, dragItem);
                }
            }
            else if (action == ItemDropAction.SetNextSibling)
            {
                for (int i = 0; i < dragItems.Length; ++i)
                {
                    ItemContainer dragItem = dragItems[i];
                    DropItemAfter(dropTarget, dragItem);
                }
            }

            UpdateSelectedItemIndex();
        }

        protected void UpdateSelectedItemIndex()
        {
            m_selectedIndex = IndexOf(SelectedItem);
        }

        /// <summary>
        /// DataBinding operation called when Items property value changed
        /// </summary>
        protected virtual void DataBind()
        {
            m_itemContainers = GetComponentsInChildren<ItemContainer>().ToList();
            if (m_items == null)
            {
                for (int i = 0; i < m_itemContainers.Count; ++i)
                {
                    Destroy(m_itemContainers[i].gameObject);
                }
            }
            else
            {
                int deltaItems = m_items.Count - m_itemContainers.Count;
                if (deltaItems > 0)
                {
                    for (int i = 0; i < deltaItems; ++i)
                    {
                        InstantiateItemContainer(m_itemContainers.Count);
                    }
                }
                else
                {
                    int newLength = m_itemContainers.Count + deltaItems;
                    for (int i = m_itemContainers.Count - 1; i >= newLength; i--)
                    {
                        DestroyItemContainer(i);
                    }
                }
            }

            for (int i = 0; i < m_items.Count; ++i)
            {
                object item = m_items[i];
                ItemContainer itemContainer = m_itemContainers[i];
                itemContainer.CanDrag = CanDrag;
                if (itemContainer != null)
                {
                    itemContainer.Item = item;
                    DataBindItem(item, itemContainer);
                }
            }
        }

        protected virtual void DataBindItem(object item, ItemContainer itemContainer)
        {
            TDataBindingArgs args = new TDataBindingArgs();
            args.Item = item;
            args.ItemPresenter = itemContainer.gameObject;
            RaiseItemDataBinding(args);
        }

        protected void RaiseItemDataBinding(TDataBindingArgs args)
        {
            if (ItemDataBinding != null)
            {
                ItemDataBinding(this, args);
            }
        }

        protected ItemContainer InstantiateItemContainer(int siblingIndex)
        {
            GameObject container = Instantiate(ItemContainerPrefab);
            container.name = "ItemContainer";
            container.transform.SetParent(Panel, false);
            container.transform.SetSiblingIndex(siblingIndex);

            ItemContainer itemContainer = InstantiateItemContainerOverride(container);
            itemContainer.CanDrag = CanDrag;
            itemContainer.LayoutElement.minWidth = m_width;
            m_itemContainers.Insert(siblingIndex, itemContainer);
            return itemContainer;
        }

        protected void DestroyItemContainer(int siblingIndex)
        {
            if (m_itemContainers == null)
            {
                return;
            }

            if (siblingIndex >= 0 && siblingIndex < m_itemContainers.Count)
            {
                DestroyImmediate(m_itemContainers[siblingIndex].gameObject);
                m_itemContainers.RemoveAt(siblingIndex);
            }
        }

        /// <summary>
        /// Overried to Instantiante derived class ItemContainer
        /// </summary>
        /// <param name="container">item container gameobject</param>
        /// <returns>component derived from ItemContainer</returns>
        protected virtual ItemContainer InstantiateItemContainerOverride(GameObject container)
        {
            ItemContainer itemContainer = container.GetComponent<ItemContainer>();
            if (itemContainer == null)
            {
                itemContainer = container.AddComponent<ItemContainer>();
            }
            return itemContainer;
        }

        private void DestroySelectedItems()
        {
            if (m_selectedItems == null)
            {
                return;
            }

            object[] selectedItems = m_selectedItems.ToArray();
            if (selectedItems.Length == 0)
            {
                return;
            }
            SelectedItems = null;

            for (int i = 0; i < selectedItems.Length; ++i)
            {
                object selectedItem = selectedItems[i];
                DestroyItem(selectedItem);
            }

            if (ItemsRemoved != null)
            {
                ItemsRemoved(this, new ItemsRemovedArgs(selectedItems));
            }
        }

        protected virtual void DestroyItem(object item)
        {
            if (m_selectedItems != null && m_selectedItems.Contains(item))
            {
                //NOTE: Selection Changed Event is not raised 
                m_selectedItems.Remove(item);
                if (m_selectedItems.Count == 0)
                {
                    m_selectedItem = null;
                    m_selectedIndex = -1;
                }
                else
                {
                    m_selectedItem = GetItemContainer(m_selectedItems[0]);
                    m_selectedIndex = IndexOf(m_selectedItem.Item);
                }
            }
            ItemContainer itemContainer = GetItemContainer(item);
            if (itemContainer != null)
            {
                int siblingIndex = itemContainer.transform.GetSiblingIndex();
                DestroyItemContainer(siblingIndex);
                m_items.Remove(item);
            }
        }
    }
}

