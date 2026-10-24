#pragma once
#include <vector>
#include <memory>
#include <string>
#include <concepts>
#include <typeindex>

struct ControlBlock
{
	void* ptr_;
	int count_;
};
template<class T>
class RefView
{
public:
	RefView(ControlBlock* blk) noexcept : blk_{ blk }
	{
	}

	T* operator->() noexcept
	{
		if (!blk_)
			return nullptr;
		return (T*)blk_->ptr_;
	}

	const T* operator->() const noexcept
	{
		if (!blk_)
			return nullptr;
		return (T*)blk_->ptr_;
	}

	void incRef() noexcept
	{
		if (blk_)
			++blk_->count_;
	}

	const std::type_info* heldType() const
	{
		if (!blk_)
			return nullptr;
		return &typeid(*(T*)blk_->ptr_);
	}

public:
	ControlBlock* blk_;
};

template<class T>
class Ref: public RefView<T>
{
public:
	Ref(ControlBlock* blk): RefView<T>(blk){}

	Ref(T*&& ptr) noexcept : RefView<T>(new ControlBlock{.ptr_ = ptr, .count_ = 1} )
	{}
	template<std::derived_from<T> U>
	Ref(Ref<U>&& ref) noexcept : RefView<T>(ref.blk_)
	{
		ref.blk_ = nullptr;
	}

	template<std::derived_from<T> U>
	Ref(const RefView<U>& ref) noexcept : RefView<T>( ref.blk_ )
	{
		this->incRef();
	}

	~Ref() noexcept
	{
		if (!this->blk_)
			return;
		--this->blk_->count_;
		if (this->blk_->count_ == 0)
		{
			delete (T*)this->blk_->ptr_;
			delete this->blk_;
		}
		this->blk_ = nullptr;
	}

	ControlBlock* grab()&&
	{
		auto blk = this->blk_;
		this->blk_ = nullptr;
		return blk;
	}

};

struct Style
{
	virtual ~Style() = default;
};

struct LineStyle: Style
{
	virtual ~LineStyle() override = default;
	double width;
};

struct SymbolStyle : Style
{
	virtual ~SymbolStyle() override = default;
	std::string path;
};

struct Layer
{
	std::vector<Ref<Style>> styles;
};

struct View
{
	std::vector<Ref<Layer>> layers;
	~View()
	{

	}
};
