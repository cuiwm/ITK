/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkForwardDifferenceOperator.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

  =========================================================================*/
#ifndef _itkForwardDifferenceOperator_txx
#define _itkForwardDifferenceOperator_txx
namespace itk
{

template <class TPixel, unsigned int VDimension, class TAllocator>
typename ForwardDifferenceOperator<TPixel, VDimension, TAllocator>
::CoefficientVector
ForwardDifferenceOperator<TPixel, VDimension, TAllocator>
::GenerateCoefficients()
{
  CoefficientVector coeff(3);

  coeff[0] = NumericTraits<ScalarValueType>::Zero;
  coeff[1] = -1.0f *  NumericTraits<ScalarValueType>::One;
  coeff[2] =  NumericTraits<ScalarValueType>::One;

  return coeff;
}
  
} // namespace itk

#endif
