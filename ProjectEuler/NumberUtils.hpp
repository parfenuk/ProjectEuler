//
//  NumberUtils.hpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 3/4/21.
//  Copyright © 2021 Miraslau Parafeniuk. All rights reserved.
//

#pragma once

#include "Definitions.hpp"
#include "Algebra.hpp"
#include "Containers.hpp"

namespace NumberUtils
{
    dd decimal_part (dd a);
    ull integer_sqrt (ull n);
    ull lower_integer_sqrt (ull n);
    ull upper_integer_sqrt (ull n);
    vint digits (ull n, int base = 10, int min_size = 1);
    ull from_digits (const vint &a, int base = 10);
    bool is_palindromic_number (ull n);
    bool same_digits (ull n, ull m);
    ull ones_mod (ull k, ull mod);
    ll code_from_v (const vint &v, const vint &matches);
    vint v_from_code (ll n, const vint &matches);
}
