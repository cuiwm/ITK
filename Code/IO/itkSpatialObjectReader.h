/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSpatialObjectReader.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSpatialObjectReader_h
#define __itkSpatialObjectReader_h

#include "itkMetaSceneConverter.h"
#include "itkSpatialObject.h"
#include "itkGroupSpatialObject.h"
#include "itkProcessObject.h"
#include "itkSceneSpatialObject.h"
#include "itkMetaEvent.h"

namespace itk
{

template <unsigned int NDimensions = 3, class PixelType = unsigned char>
class SpatialObjectReader : public Object
{
public:

  /** SmartPointer typedef support */
  typedef SpatialObjectReader Self;
  typedef SmartPointer<Self> Pointer;

  typedef SpatialObject<NDimensions> SpatialObjectType;
  typedef typename SpatialObjectType::Pointer SpatialObjectPointer;

  typedef GroupSpatialObject<NDimensions> GroupType;
  typedef typename GroupType::Pointer GroupPointer;
  
  typedef SceneSpatialObject<NDimensions> SceneType;
  typedef typename SceneType::Pointer ScenePointer;

   /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  typedef Object Superclass;
  itkTypeMacro(Self, Superclass);

  /** Load a scene file. */
  void Update(void);

  /** Set the filename  */
  itkSetStringMacro(FullFileName); // will be depreciated
  itkSetStringMacro(FileName);

  /** Get the filename */
  itkGetStringMacro(FullFileName); // will be depreciated
  itkGetStringMacro(FileName);

  /** Get the output */
  ScenePointer GetScene(void) {return m_Scene;}
  GroupPointer GetGroup(void) {return m_Group;}

  /** Set/GetEvent */
  const MetaEvent* GetEvent() {return m_MetaToSpatialConverter.GetEvent();}
  void SetEvent(MetaEvent* event) {m_MetaToSpatialConverter.SetEvent(event);} 

protected:
  std::string m_FullFileName;
  std::string m_FileName;

  SpatialObjectReader();
  virtual ~SpatialObjectReader();

private:

  ScenePointer m_Scene;
  GroupPointer m_Group;
  MetaSceneConverter<NDimensions,PixelType> m_MetaToSpatialConverter;
};

} // namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSpatialObjectReader.txx"
#endif

#endif // __itkSpatialObjectReader_h
