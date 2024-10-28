
using System;


namespace Ezzoo {


    public class Main
    {
        public float MyPublicFloatVar = 5.0f;
        public int MyPublicIntVar = 5;

        public Main ()
        {
            Console.WriteLine("MainConstructor!!");
        }


        public void PrintFloatVar()
        {
            Console.WriteLine("MyPublicFloatVar = {0:F}", MyPublicFloatVar);
        } 

        public void PrintIntVar(int x, int y)
        {
            Console.WriteLine("x = {0}, y = {1}", x, y);
        }

        public void PrintString (string str)
        {
            Console.WriteLine(str);
        }

        public void IncrementFloatVar(float value)
        {
            MyPublicFloatVar += value;
        }



    }
}