﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LibStrawberry.BindingBase
{
	internal static class Native
	{
		public static UIntPtr GetPointer(this object obj) {
			UIntPtr ptr = UIntPtr.Zero;
			if (obj == null) {
				return ptr;
			}
#if UNSAFE
			unsafe {
				TypedReference tr = __makeref(obj);
				ptr = *((UIntPtr*)(&tr));
			}
#endif
			return ptr;
		}
	}
}
