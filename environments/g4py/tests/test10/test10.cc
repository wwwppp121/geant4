// $Id: test10.cc,v 1.2 2006-04-25 07:54:29 kmura Exp $
// $Name: not supported by cvs2svn $
// ====================================================================
//   test10.cc
//
//   test for call by-reference
//
//                                         2005 Q
// ====================================================================
#include <iostream>

class AClass {
public:
  AClass() {
    std::cout << "*** AClass is created..." << this
	      << std::endl;
  }

  ~AClass() {
    std::cout << "*** AClass is deleted..." << this
	      << std::endl;
  }
};

class BClass {
public:
  BClass() {
    std::cout << "*** BClass is created..." << this
	      << std::endl;
  }
  ~BClass() {
    std::cout << "*** BClass is deleted..." << this
	      << std::endl;
  }
};

class XBase {
public:
  XBase() {}
  ~XBase() {}

  virtual void VMethodA(const AClass* a) {
    std::cout << "*** XBase::VMethod...A() is called." << std::endl;
  }

  virtual void VMethodB(const BClass* b) {
    std::cout << "*** XBase::VMethod...B() is called." << std::endl;
  }
};


class ZClass {
private:
  AClass a;
  BClass b;
  XBase* xbase;

public:
  ZClass() : xbase(0) { }
  ~ZClass() { }

  void SetXBase(XBase* base) {
    xbase= base;
  }

  void Process() {
    xbase-> VMethodA(&a);
    xbase-> VMethodB(&b);
  }
};


// Boost.Python...
#include <boost/python.hpp>

using namespace boost::python;

// call backs
struct CB_XBase : XBase, wrapper<XBase> {
  void VMethodA(const AClass* a) {
    if(const override& f= get_override("VMethodA"))
      f(a);
    else
      XBase::VMethodA(a);
  }

  void d_VMethodA(const AClass* a) {
    XBase::VMethodA(a);
  }

  //
  void VMethodB(const BClass* b) {
    if(const override& f= get_override("VMethodB"))
      f(b);
    else
      XBase::VMethodB(b);
  }
  
  void d_VMethodB(const BClass* b) {
    XBase::VMethodB(b);
  }
};


BOOST_PYTHON_MODULE(test10)
{
  class_<AClass, AClass*>("AClass", "a class")
    .def(init<>())
    ;

  class_<BClass>("BClass", "b class")
    .def(init<>())
    ;

  class_<CB_XBase, boost::noncopyable>("XBase", "xbase class")
    .def(init<>())
    .def("VMethodA", &XBase::VMethodA, &CB_XBase::d_VMethodA)
    .def("VMethodB", &XBase::VMethodB, &CB_XBase::d_VMethodB)
    ;

  class_<ZClass>("ZClass", "z class")
    .def(init<>())
    .def("SetXBase", &ZClass::SetXBase)
    .def("Process", &ZClass::Process)
    ;
}

