#pragma once
#include "util/optional.h"


//! optional property macro
#define DEFINE_OPTION(TYPE, NAME) \
    private: \
    util::optional< TYPE > _ ## NAME ; \
    public: \
    util::optional< TYPE >& NAME () { return _ ## NAME; } \
    const util::optional< TYPE >& NAME () const { return _ ## NAME; }


//! ref_ptr property macro
#define DEFINE_OPTION_SHAREDPTR(TYPE, NAME) \
    private: \
    std::shared_ptr< TYPE > _ ## NAME ; \
    public: \
    std::shared_ptr< TYPE >& NAME () { return _ ## NAME; } \
    const std::shared_ptr< TYPE >& NAME () const { return _ ## NAME; }

//! vector property macro
#define DEFINE_OPTION_VECTOR(TYPE, NAME) \
    private: \
    std::vector< TYPE > _ ## NAME ; \
    public: \
    std::vector< TYPE >& NAME () { return _ ## NAME; } \
    const std::vector< TYPE >& NAME () const { return _ ## NAME; }

#define DEFINE_PROPERTY_IMPL(CLASS, TYPE, FUNC, OPTION) \
    void CLASS ::set ## FUNC (const TYPE & value) { options(). OPTION () = value; }\
    const TYPE & CLASS ::get ## FUNC () const { return options(). OPTION ().get(); }

//! property macro
#define DEFINE_PROPERTY(TYPE, NAME) \
    private: \
    TYPE _ ## NAME ; \
    public: \
    TYPE & NAME () { return _ ## NAME; } \
    TYPE & NAME ## _mutable() { return _ ## NAME; } \
    const TYPE & NAME () const { return _ ## NAME; }

//! const property macro
#define DEFINE_PROPERTY_CONST(TYPE, NAME) \
    private: \
    TYPE _ ## NAME ; \
    protected: \
    TYPE & NAME () { return _ ## NAME; } \
    TYPE & NAME ## _mutable() { return _ ## NAME; } \
    public: \
    const TYPE & NAME () const { return _ ## NAME; }



#define DEFINE_SINGLETON(TYPE) \
public:                        \
static TYPE& instance() {      \
	static TYPE s_instance;    \
	return s_instance;          \
};                             \
static void destoryInstance();


#define DEFINE_SINGLETON_PLUGIN(TYPE)           \
protected:                                     	\
	TYPE();										\
public:  										\
	virtual ~TYPE() = default;					\
static TYPE* instance() {     					\
	return m_pInstance;           				\
};												\
static void setCurrent(TYPE* pInstance) {		\
	m_pInstance = pInstance;					\
};												\
protected:										\
	static TYPE* m_pInstance;			

#define DEFINE_SINGLETON_PLUGIN_CPP(TYPE)       \
	TYPE* TYPE::m_pInstance = nullptr;	               
                              
  

//plugin的 cpp中使用如下宏
#define REGISTER_DEFINE_SINGLETON_PLUGIN(basename,classname) \
    extern "C" void util_##ext(void) {} \
    static util::RegisterSingletonPluginProxy<basename,classname> g_proxy_##classname;




