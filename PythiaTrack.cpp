// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME PythiaTrack

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "PythiaTrack.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_PythiaTrack(void *p = 0);
   static void *newArray_PythiaTrack(Long_t size, void *p);
   static void delete_PythiaTrack(void *p);
   static void deleteArray_PythiaTrack(void *p);
   static void destruct_PythiaTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PythiaTrack*)
   {
      ::PythiaTrack *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PythiaTrack >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PythiaTrack", ::PythiaTrack::Class_Version(), "PythiaTrack.h", 6,
                  typeid(::PythiaTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PythiaTrack::Dictionary, isa_proxy, 4,
                  sizeof(::PythiaTrack) );
      instance.SetNew(&new_PythiaTrack);
      instance.SetNewArray(&newArray_PythiaTrack);
      instance.SetDelete(&delete_PythiaTrack);
      instance.SetDeleteArray(&deleteArray_PythiaTrack);
      instance.SetDestructor(&destruct_PythiaTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PythiaTrack*)
   {
      return GenerateInitInstanceLocal((::PythiaTrack*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PythiaTrack*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr PythiaTrack::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PythiaTrack::Class_Name()
{
   return "PythiaTrack";
}

//______________________________________________________________________________
const char *PythiaTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PythiaTrack*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PythiaTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PythiaTrack*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PythiaTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PythiaTrack*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PythiaTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PythiaTrack*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void PythiaTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class PythiaTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PythiaTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(PythiaTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PythiaTrack(void *p) {
      return  p ? new(p) ::PythiaTrack : new ::PythiaTrack;
   }
   static void *newArray_PythiaTrack(Long_t nElements, void *p) {
      return p ? new(p) ::PythiaTrack[nElements] : new ::PythiaTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_PythiaTrack(void *p) {
      delete ((::PythiaTrack*)p);
   }
   static void deleteArray_PythiaTrack(void *p) {
      delete [] ((::PythiaTrack*)p);
   }
   static void destruct_PythiaTrack(void *p) {
      typedef ::PythiaTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PythiaTrack

namespace {
  void TriggerDictionaryInitialization_PythiaTrack_Impl() {
    static const char* headers[] = {
"PythiaTrack.h",
0
    };
    static const char* includePaths[] = {
"/Volumes/data1/star1/jdb12/vendor/pythia_root_6.06.02/root-6.06.02/include",
"/home/jdb12/work/pythia/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "PythiaTrack dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$PythiaTrack.h")))  PythiaTrack;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "PythiaTrack dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "PythiaTrack.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"PythiaTrack", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("PythiaTrack",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_PythiaTrack_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_PythiaTrack_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_PythiaTrack() {
  TriggerDictionaryInitialization_PythiaTrack_Impl();
}
