/*!
 * \file computeLimiters.cpp
 * \brief Implementation of compute limiters function.
 * \author P. Gomes
 * \version 7.0.0 "Blackbird"
 *
 * SU2 Project Website: https://su2code.github.io
 *
 * The SU2 Project is maintained by the SU2 Foundation
 * (http://su2foundation.org)
 *
 * Copyright 2012-2019, SU2 Contributors (cf. AUTHORS.md)
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include "../../../Common/include/omp_structure.hpp"
#include "../../include/solver_structure.hpp"
#include "../../include/limiters/CLimiterDetails.hpp"
#include "../../include/limiters/computeLimiters_impl.hpp"


#define INSTANTIATE(KIND) \
computeLimiters_impl<FieldType, GradientType, KIND>(solver, kindMpiComm, \
  kindPeriodicComm1, kindPeriodicComm2, geometry, config, varBegin, \
  varEnd, field, gradient, fieldMin, fieldMax, limiter)


template<class FieldType, class GradientType>
void computeLimiters(ENUM_LIMITER LimiterKind,
                     CSolver* solver,
                     MPI_QUANTITIES kindMpiComm,
                     PERIODIC_QUANTITIES kindPeriodicComm1,
                     PERIODIC_QUANTITIES kindPeriodicComm2,
                     CGeometry& geometry,
                     CConfig& config,
                     size_t varBegin,
                     size_t varEnd,
                     const FieldType& field,
                     const GradientType& gradient,
                     FieldType& fieldMin,
                     FieldType& fieldMax,
                     FieldType& limiter)
{
  switch (LimiterKind) {
    case NO_LIMITER:
    {
      SU2_OMP_PARALLEL
      {
        SU2_OMP_FOR_STAT(512)
        for(size_t iPoint = 0; iPoint < geometry.GetnPoint(); ++iPoint)
          for(size_t iVar = varBegin; iVar < varEnd; ++iVar)
           limiter(iPoint, iVar) = 1.0;
      }
      break;
    }
    case BARTH_JESPERSEN:
    {
      INSTANTIATE(BARTH_JESPERSEN);
      break;
    }
    case VENKATAKRISHNAN:
    {
      INSTANTIATE(VENKATAKRISHNAN);
      break;
    }
    case VENKATAKRISHNAN_WANG:
    {
      INSTANTIATE(VENKATAKRISHNAN_WANG);
      break;
    }
    case WALL_DISTANCE:
    {
      INSTANTIATE(WALL_DISTANCE);
      break;
    }
    case SHARP_EDGES:
    {
      INSTANTIATE(SHARP_EDGES);
      break;
    }
    default:
    {
      SU2_MPI::Error("Unknown limiter type.", CURRENT_FUNCTION);
      break;
    }
  }
}

/*--- Explicit instantiations for required FieldType and GradientType. ---*/

template void computeLimiters<CVariable::MatrixType, CVariable::VectorOfMatrix>(ENUM_LIMITER,
  CSolver*, MPI_QUANTITIES, PERIODIC_QUANTITIES, PERIODIC_QUANTITIES, CGeometry&, CConfig&,
  size_t, size_t, const CVariable::MatrixType&, const CVariable::VectorOfMatrix&,
  CVariable::MatrixType&, CVariable::MatrixType&, CVariable::MatrixType&);

#undef INSTANTIATE
