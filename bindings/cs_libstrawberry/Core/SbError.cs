﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LibStrawberry.BindingBase;

namespace LibStrawberry.Core
{
	public static class SbError
	{
		public static uint Get() {
			return Imports.sb_error_get();
		}

		public static uint GetParam() {
			return Imports.sb_error_get_param();
		}

		public static void Print() {
			Imports.sb_error_print();
		}
	}
}
