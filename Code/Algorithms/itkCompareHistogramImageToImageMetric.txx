/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkCompareHistogramImageToImageMetric.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCompareHistogramImageToImageMetric_txx
#define __itkCompareHistogramImageToImageMetric_txx

#include "itkCompareHistogramImageToImageMetric.h"
#include "itkHistogram.h"

namespace itk
{
template <class TFixedImage, class TMovingImage>
CompareHistogramImageToImageMetric<TFixedImage, TMovingImage>::
CompareHistogramImageToImageMetric() 
{
  m_TrainingFixedImage        = 0; // has to be provided by the user.
  m_TrainingMovingImage       = 0; // has to be provided by the user.
  m_TrainingTransform         = 0; // has to be provided by the user.
  m_TrainingInterpolator      = 0; // has to be provided by the user.
  m_TrainingHistogram         = 0; // either provided by the user or created
}

template <class TFixedImage, class TMovingImage>
void 
CompareHistogramImageToImageMetric<TFixedImage, TMovingImage>
::Initialize()  throw (ExceptionObject)
{
  Superclass::Initialize();

  if (!m_TrainingHistogram)
    {
    FormTrainingHistogram();
    }
}

template <class TFixedImage, class TMovingImage>
void 
CompareHistogramImageToImageMetric<TFixedImage, TMovingImage>
::FormTrainingHistogram() throw (ExceptionObject)
{
  // Check to make sure everything is set
  if( !m_TrainingTransform )
    {
    itkExceptionMacro(<<"Training Transform is not present");
    }

  if( !m_TrainingInterpolator )
    {
    itkExceptionMacro(<<"Training Interpolator is not present");
    }

  if( !m_TrainingMovingImage )
    {
    itkExceptionMacro(<<"Training MovingImage is not present");
    }

// If the image is provided by a source, update the source.
  if( m_TrainingMovingImage->GetSource() )
    {
    m_TrainingMovingImage->GetSource()->Update();
    }

  if( !m_TrainingFixedImage )
    {
    itkExceptionMacro(<<"Training FixedImage is not present");
    }

// If the image is provided by a source, update the source.
  if( m_TrainingFixedImage->GetSource() )
    {
    m_TrainingFixedImage->GetSource()->Update();
    }

  if( m_TrainingFixedImageRegion.GetNumberOfPixels() == 0 )
    {
    itkExceptionMacro(<<"TrainingFixedImageRegion is empty");
    }

// Make sure the FixedImageRegion is within the FixedImage buffered region
  if ( !m_TrainingFixedImageRegion.Crop( m_TrainingFixedImage->GetBufferedRegion() ))
    {
    itkExceptionMacro(<<"TrainingFixedImageRegion does not overlap the training fixed image buffered region" );
    }

  this->m_TrainingInterpolator->SetInputImage(GetTrainingMovingImage());
//
// Create the exact histogram structure as the one to be used
// to evaluate the metric. This code is mostly copied
// from itkHistogramImageToImageMetric
//
  this->m_TrainingHistogram = HistogramType::New();
  this->m_TrainingHistogram->Initialize(this->Superclass::m_HistogramSize,
                                        this->Superclass::m_LowerBound,
                                        this->Superclass::m_UpperBound);
  typedef itk::ImageRegionConstIteratorWithIndex<FixedImageType>
    TrainingFixedIteratorType;
  typename FixedImageType::IndexType index;
  typename FixedImageType::RegionType fixedRegion;

  TrainingFixedIteratorType ti(this->m_TrainingFixedImage,
                               this->m_TrainingFixedImageRegion);

  int NumberOfPixelsCounted = 0;

  ti.GoToBegin();
  while (!ti.IsAtEnd())
    {
    index = ti.GetIndex();
    if (this->m_TrainingFixedImageRegion.IsInside(index) &&
        (!this->Superclass::GetUsePaddingValue() ||
         (this->Superclass::GetUsePaddingValue() && 
          ti.Get() > this->Superclass::GetPaddingValue())))
      {
      typename Superclass::InputPointType inputPoint;
      this->m_TrainingFixedImage->
        TransformIndexToPhysicalPoint(index, inputPoint);

      typename Superclass::OutputPointType transformedPoint =
        this->m_TrainingTransform->TransformPoint(inputPoint);

      if (this->m_TrainingInterpolator->IsInsideBuffer(transformedPoint))
        {
        const RealType TrainingMovingValue =
          this->m_TrainingInterpolator->Evaluate(transformedPoint);
        const RealType TrainingFixedValue = ti.Get();
        NumberOfPixelsCounted++;

        typename HistogramType::MeasurementVectorType sample;
        sample[0] = TrainingFixedValue;
        sample[1] = TrainingMovingValue;
        this->m_TrainingHistogram->IncreaseFrequency(sample, 1);
        }
      }

    ++ti;
    }

  if (NumberOfPixelsCounted == 0)
    {
    itkExceptionMacro(<< "All the points mapped to outside of the Training moving \
age");
    }
}

template <class TFixedImage, class TMovingImage>
void CompareHistogramImageToImageMetric<TFixedImage, TMovingImage>::
PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "TrainingFixedImage: ";
  if (m_TrainingFixedImage  == 0)
    {
    os << 0 << std::endl;
    }
  else
    {
    os << m_TrainingFixedImage << std::endl;
    }

  os << indent << "TrainingMovingImage: ";
  if (m_TrainingMovingImage  == 0)
    {
    os << 0 << std::endl;
    }
  else
    {
    os << m_TrainingMovingImage << std::endl;
    }
  os << indent << "TrainingTransform: ";
  if (m_TrainingTransform    == 0)
    {
    os << 0 << std::endl;
    }
  else
    {
    os << m_TrainingTransform << std::endl;
    }
  os << indent << "TrainingInterpolator: ";
  if (m_TrainingInterpolator == 0)
    {
    os << 0 << std::endl;
    }
  else
    {
    os << m_TrainingInterpolator << std::endl;
    }
  os << indent << "TrainingHistogram: ";
  if (m_TrainingHistogram    == 0)
    {
    os << 0 << std::endl;
    }
  else
    {
    os << m_TrainingHistogram << std::endl;
    }
  os << indent << "TrainingFixedImageRegion: " << m_TrainingFixedImageRegion
     << std::endl;
}


} // End namespace itk

#endif // itkCompareHistogramImageToImageMetric_txx
