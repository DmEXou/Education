#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <algorithm>//
#include <stdexcept>//
#include <iostream>//

template <typename Type>
class SingleLinkedList {
public:
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

public:
    SingleLinkedList<Type>()
    {
    };

    ~SingleLinkedList<Type>()
    {
        Clear();
    }

public:
    SingleLinkedList(std::initializer_list<Type> values) {
        SingleLinkedList<Type> tmp;
        for (const auto& val : values) {
            tmp.PushFront(val);
        }
        for (const auto& val : tmp) {
            this->PushFront(val);
        }
        tmp.Clear();
    }

    SingleLinkedList(const SingleLinkedList& other) {
        SingleLinkedList<Type> tmp;
        for (const auto& val : other) {
            tmp.PushFront(val);
        }
        for (const auto& val : tmp) {
            this->PushFront(val);
        }
        tmp.Clear();
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        SingleLinkedList <Type> tmp(rhs);
        this->swap(tmp);
        tmp.Clear();
        return *this;
    }

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) {
            node_ = node;
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return (this->node_ == rhs.node_);
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return (this->node_ != rhs.node_);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return (this->node_ == rhs.node_);
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return (this->node_ != rhs.node_);
        }

        BasicIterator& operator++() noexcept {
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            Type* val = &node_->value;
            return val;
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{ &head_ };
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        
        return ConstIterator{ const_cast<Node*>(&head_) };
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{ const_cast<Node*>(&head_) };
    }

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator{ nullptr };
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{ nullptr };
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{ nullptr };
    }

public:
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return (GetSize() == 0);
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        SingleLinkedList<Type> tmp;
        if(pos != this->before_begin()){
            for (auto it = this->begin(); it != pos; ++it) {
                tmp.PushFront(*it);
                this->PopFront();
            }
        }
        this->PushFront(value);
        Iterator it = Iterator(this->head_.next_node);
        if (pos != this->before_begin()) {
            for (const auto& val : tmp) {
                this->PushFront(val);
            }
        }
        tmp.Clear();
        return {it};
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void PopFront() noexcept {
        if (head_.next_node != nullptr) {
            auto prev = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = prev;
            --size_;
        }
    }

    void Clear() noexcept {
        int size = GetSize();
        for (int i = 0; i < size; ++i) {
            if (head_.next_node != nullptr) {
                auto prev = head_.next_node->next_node;
                delete head_.next_node;
                head_.next_node = prev;
                --size_;
            }
        }
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        SingleLinkedList<Type> tmp;
        for (auto it = this->begin(); it != pos; ++it) {
            tmp.PushFront(*it);
            this->PopFront();
        }
        this->PopFront();
        Iterator it = Iterator(this->head_.next_node);
        for (const auto& val : tmp) {
            this->PushFront(val);
        }
        tmp.Clear();
        --size_;
        return { it };
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(this->size_, other.size_);
        std::swap(this->head_.next_node, other.head_.next_node);

    }

private:
    Node head_;
    size_t size_ = 0;
};
template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if ((lhs.GetSize() == rhs.GetSize()) && (std::equal(lhs.begin(), lhs.end(), rhs.begin()))) {
        return true;
    } else {
        return false;
    }
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return ((lhs < rhs) || (lhs == rhs));
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (rhs < lhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return ((lhs > rhs) || (lhs == rhs));;
}

void Test4() {
    struct DeletionSpy {
        ~DeletionSpy() {
            if (deletion_counter_ptr) {
                ++(*deletion_counter_ptr);
            }
        }
        int* deletion_counter_ptr = nullptr;
    };

    // Проверка PopFront
    {
        SingleLinkedList<int> numbers{ 3, 14, 15, 92, 6 };
        numbers.PopFront();
        assert((numbers == SingleLinkedList<int>{14, 15, 92, 6}));

        SingleLinkedList<DeletionSpy> list;
        list.PushFront(DeletionSpy{});
        int deletion_counter = 0;
        list.begin()->deletion_counter_ptr = &deletion_counter;
        assert(deletion_counter == 0);
        list.PopFront();
        assert(deletion_counter == 1);
    }

    // Доступ к позиции, предшествующей begin
    {
        SingleLinkedList<int> empty_list;
        const auto& const_empty_list = empty_list;
        assert(empty_list.before_begin() == empty_list.cbefore_begin());
        assert(++empty_list.before_begin() == empty_list.begin());
        assert(++empty_list.cbefore_begin() == const_empty_list.begin());

        SingleLinkedList<int> numbers{ 1, 2, 3, 4 };
        const auto& const_numbers = numbers;
        assert(numbers.before_begin() == numbers.cbefore_begin());
        assert(++numbers.before_begin() == numbers.begin());
        assert(++numbers.cbefore_begin() == const_numbers.begin());
    }

    // Вставка элемента после указанной позиции
    {  // Вставка в пустой список
        {
            SingleLinkedList<int> lst;
            const auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);
            assert((lst == SingleLinkedList<int>{123}));
            assert(inserted_item_pos == lst.begin());
            assert(*inserted_item_pos == 123);
        }

        // Вставка в непустой список
        {
            SingleLinkedList<int> lst{ 1, 2, 3 };
            auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);

            assert(inserted_item_pos == lst.begin());
            assert(inserted_item_pos != lst.end());
            assert(*inserted_item_pos == 123);
            assert((lst == SingleLinkedList<int>{123, 1, 2, 3}));

            inserted_item_pos = lst.InsertAfter(lst.begin(), 555);
            assert(++SingleLinkedList<int>::Iterator(lst.begin()) == inserted_item_pos);
            assert(*inserted_item_pos == 555);
            assert((lst == SingleLinkedList<int>{123, 555, 1, 2, 3}));
        };
    }

    // Вспомогательный класс, бросающий исключение после создания N-копии
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                }
                else {
                    --(*countdown_ptr);
                }
            }
        }
        // Присваивание элементов этого типа не требуется
        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
        // Адрес счётчика обратного отсчёта. Если не равен nullptr, то уменьшается при каждом копировании.
        // Как только обнулится, конструктор копирования выбросит исключение
        int* countdown_ptr = nullptr;
    };

    // Проверка обеспечения строгой гарантии безопасности исключений
    {
        bool exception_was_thrown = false;
        for (int max_copy_counter = 10; max_copy_counter >= 0; --max_copy_counter) {
            SingleLinkedList<ThrowOnCopy> list{ ThrowOnCopy{}, ThrowOnCopy{}, ThrowOnCopy{} };
            try {
                int copy_counter = max_copy_counter;
                list.InsertAfter(list.cbegin(), ThrowOnCopy(copy_counter));
                assert(list.GetSize() == 4u);
            }
            catch (const std::bad_alloc&) {
                exception_was_thrown = true;
                assert(list.GetSize() == 3u);
                break;
            }
        }
        assert(exception_was_thrown);
    }

    // Удаление элементов после указанной позиции
    {
        {
            SingleLinkedList<int> lst{ 1, 2, 3, 4 };
            const auto& const_lst = lst;
            const auto item_after_erased = lst.EraseAfter(const_lst.cbefore_begin());
            assert((lst == SingleLinkedList<int>{2, 3, 4}));
            assert(item_after_erased == lst.begin());
        }
        {
            SingleLinkedList<int> lst{ 1, 2, 3, 4 };
            const auto item_after_erased = lst.EraseAfter(lst.cbegin());
            assert((lst == SingleLinkedList<int>{1, 3, 4}));
            assert(item_after_erased == (++lst.begin()));
        }
        {
            SingleLinkedList<int> lst{ 1, 2, 3, 4 };
            const auto item_after_erased = lst.EraseAfter(++(++lst.cbegin()));
            assert((lst == SingleLinkedList<int>{1, 2, 3}));
            assert(item_after_erased == lst.end());
        }
        {
            SingleLinkedList<DeletionSpy> list{ DeletionSpy{}, DeletionSpy{}, DeletionSpy{} };
            auto after_begin = ++list.begin();
            int deletion_counter = 0;
            after_begin->deletion_counter_ptr = &deletion_counter;
            assert(deletion_counter == 0u);
            list.EraseAfter(list.cbegin());
            assert(deletion_counter == 1u);
        }
    }
}

int main() {
    Test4();
}