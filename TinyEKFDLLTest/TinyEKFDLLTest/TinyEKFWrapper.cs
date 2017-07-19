using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TinyEKFDLLTest
{
    static class TinyEKFWrapper
    {
        private const string DllFilePath = @"E:\users\administrator\Source\Repos\TinyEKF\TinyEKF_C\Debug\TinyEKFDLL.dll";



        
[System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential)]
public struct ekf_t {
    
    /// int
    public int n;
    
    /// int
    public int m;
    
    /// double[3]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=3, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] x;
    
    /// double[9]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=9, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] P;
    
    /// double[9]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=9, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] Q;
    
    /// double[9]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=9, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] R;
    
    /// double[9]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=9, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] G;
    
    /// double[9]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=9, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] F;
    
    /// double[9]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=9, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] H;
    
    /// double[9]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=9, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] Ht;
    
    /// double[9]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=9, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] Ft;
    
    /// double[9]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=9, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] Pp;
    
    /// double[3]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=3, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] fx;
    
    /// double[3]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=3, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] hx;
    
    /// double[9]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=9, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] tmp0;
    
    /// double[9]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=9, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] tmp1;
    
    /// double[9]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=9, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] tmp2;
    
    /// double[9]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=9, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] tmp3;
    
    /// double[9]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=9, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] tmp4;
    
    /// double[3]
    [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst=3, ArraySubType=System.Runtime.InteropServices.UnmanagedType.R8)]
    public double[] tmp5;
}

        [DllImport(DllFilePath, CallingConvention = CallingConvention.Cdecl)]
        private static extern void ekf_init(IntPtr v, int n, int m);
        public static void EKFInit(IntPtr v, int n, int m)
        {
            ekf_init(v, n, m);
        }

        [DllImport(DllFilePath, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ekf_step(IntPtr v, double[] z);

        public static int EKFStep(IntPtr v, double[] z)
        {
           return ekf_step(v, z);
        }


        [DllImport(DllFilePath, CallingConvention = CallingConvention.Cdecl)]
        private extern static void init(IntPtr ekf, double[] z);
        public static void Init(IntPtr ekf, double[] z)
        {
            init(ekf, z);
        }


        [DllImport(DllFilePath, CallingConvention = CallingConvention.Cdecl)]
        private static extern void model(IntPtr ekf, double odom);

        public static void Model(IntPtr ekf, double odom)
        {
            model(ekf,odom);
        }






    }
}
