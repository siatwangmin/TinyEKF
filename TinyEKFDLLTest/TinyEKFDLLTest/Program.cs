using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using csmatio;
using csmatio.io;
using csmatio.types;

namespace TinyEKFDLLTest
{
    class Program
    {
        static void Main(string[] args)
        {
            csmatio.io.MatFileReader mfr = new MatFileReader(@"E:\Users\Administrator\Source\Repos\TinyEKF\TinyEKFDLLTest\TinyEKFDLLTest\big-circle-ekf-dll.mat");

            
            var x = (mfr.GetMLArray("x") as MLDouble).GetArray().ToList();
            var y = (mfr.GetMLArray("y") as MLDouble).GetArray().ToList();
            var orth_theta = (mfr.GetMLArray("orth_theta") as MLDouble).GetArray().ToList();
            var delta_odom = (mfr.GetMLArray("delta_odom") as MLDouble).GetArray().ToList();

            int length = mfr.Data[0].Size;
            
            List<double []> z = new List<double []>(length);
            List<double []> x_fusion = new List<double[]>(length);

            for (int i = 0; i < length; i++)
            {
                //var temp = x[i][0];
                double[] temp = new[] { x[i][0], y[i][0] , orth_theta[i][0]};
                z.Add(temp);
            }

            x_fusion.Add(new[] { z[0][0], z[0][1], z[0][2] });

            TinyEKFWrapper.ekf_t ekf = new TinyEKFWrapper.ekf_t();
            IntPtr ekfIntPtr = Marshal.AllocHGlobal(Marshal.SizeOf(ekf));

            try
            {
                
                Marshal.StructureToPtr(ekf, ekfIntPtr, false);

                TinyEKFWrapper.EKFInit(ekfIntPtr, 3, 3);

                TinyEKFWrapper.Init(ekfIntPtr, z[0]);

                //var a = (TinyEKFWrapper.ekf_t)Marshal.PtrToStructure(ekfIntPtr, ekf.GetType());


                for (int i = 1; i < length; i++)
                {
                    TinyEKFWrapper.Model(ekfIntPtr, delta_odom[i][0]);

                    //a = (TinyEKFWrapper.ekf_t)Marshal.PtrToStructure(ekfIntPtr, ekf.GetType());

                    TinyEKFWrapper.EKFStep(ekfIntPtr, z[i]);

                    //a = (TinyEKFWrapper.ekf_t)Marshal.PtrToStructure(ekfIntPtr, ekf.GetType());



                    var temp = (TinyEKFWrapper.ekf_t) Marshal.PtrToStructure(ekfIntPtr, ekf.GetType());

                    x_fusion.Add(new[] {temp.x[0], temp.x[1], temp.x[2]});
                }
            }
            finally
            {
                Marshal.DestroyStructure(ekfIntPtr, ekf.GetType());
            }


            StreamWriter file = new StreamWriter(@"E:\Document\EyooProject\2.Vanke\odom\b-circle-dll.dat");

            foreach (var data in x_fusion)
            {
                foreach (var d in data)
                {
                    file.Write(d.ToString() + ",");
                }
                file.WriteLine();
            }

            file.Close();
            
          //  TinyEKFWrapper.Print();
            Console.WriteLine("the last postion [{0}\t{1}\t{2}]", x_fusion[length - 1][0], x_fusion[length - 1][1], x_fusion[length - 1][2]);
            Console.ReadLine();
        }
    }
}
