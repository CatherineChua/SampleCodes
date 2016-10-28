using System;

public class Matrix
{
    private int row;
    private int col;
    private int length;
    private string value;
//    private int ArrayLength;
    private int[] indexOfNums;
    private char[] myChar;
    private char seperate;

    private char[] SNumbers;
    private int[] NumberFill;
    private int[,] FinalMatrix;

    public Matrix(string v, char s)
    {
        value = v;
        myChar = v.ToCharArray();
        seperate = s;
        length = v.Length;
        indexOfNums = IndexOfNums();
        row = NumOfRows();
        col = NumOfCols();
  //      ArrayLength = LengthWithoutExtras();
        SNumbers = ToNewMyChar();
        NumberFill = Numbers();
        FinalMatrix = fillMatrix();
    }


    public int LengthWithoutExtras()
    {
        int count = 0;
        if (ProperStart())
        {
            for (int i = 0; i < length; i++)
            {
                if (myChar[i] == ' ' || myChar[i] == seperate || myChar[i] == '[')
                {
                    count = count + 0;
                }
                else if (myChar[i] == ']' || myChar[i] == '[')
                {
                    i = length;
                }
                else
                {
                    count++;
                }
            }
  //          Console.Write("LengthWithoutExtras= ");
            return count;
        }
        else
        {
            Console.Write("ERROR: LengthWithoutExtras");
            return 1;
        }
    }

    public int[] IndexOfNums()
    {
        int l = LengthWithoutExtras();
        int temp = 0;
        int[] nums = new int[l];
        if (ProperStart())
        {
            for (int i = 0; i < length; i++)
            {
                if (myChar[i] == ' ' || myChar[i] == seperate || myChar[i] == '[')
                {
                    temp = temp + 0;
                }
                else if (myChar[i] == ']' || myChar[i] == '[')
                {
                    i = length;
                }
                else
                {
                    nums[temp] = i;
                    temp++;
                }
            }
            return nums;
        }
        else
        {
            Console.WriteLine("ERROR: IndexOfNums, Improper matrix format");
            return nums;
        }

    }

    public void printIndexOfNums()
    {
        Console.Write("printIndexOfNums: ");
        int l = LengthWithoutExtras();
        for(int i = 0; i < l; i++)
        {
            Console.Write("{0}, ",indexOfNums[i]);
        }
    }

   public int NumOfRows()
    {
       int count = 1;
       if (ProperStart())
        {
            for (int i = 0; i < length; i++)
            {
                if (myChar[i] == ' ' || myChar[i] == '[')
                {
                    count = count + 0;
                }
                else if (myChar[i] == ']' || myChar[i] == '[')
                {
                    i = length;
                }
                else if(myChar[i] == seperate)
                {
                    count++;
                }
                else
                {
                    count = count + 0;
                }
            }
 
            return count;
        }
        else
        {
            Console.Write("ERROR: NumOfROws: ");
            return 0;
        }
    }


   public int NumOfCols()
   {
       int count = 0;
       if (ProperStart())
       {
           for (int i = 0; i < length; i++)
           {
               if (myChar[i] == ' ' || myChar[i] == '[')
               {
                   count = count + 0;
               }
               else if (myChar[i] == ']' || myChar[i] == ';')
               {
                   i = length;
               }
               else
               {
                   count++;
               }
           }

           return count;
       }
       else
       {
           Console.Write("ERROR: NumOfCols: ");
           return 0;
       }
   }


    public char[] ToNewMyChar()
   {
       int k = 0;
       int l = LengthWithoutExtras();
       char[] newMyChar = new char[l]; 
        for(int i = 0; i < l; i++)
        {
            k = indexOfNums[i];
            newMyChar[i] = myChar[k];
        }
        return newMyChar;
   }

    public void printToNewMyChar()
    {
        Console.Write("printToNewMyChar: ");
        int l = LengthWithoutExtras();
        for (int i = 0; i < l; i++)
        {
            Console.Write("{0}, ", SNumbers[i]);
        }
    }

    public int[] Numbers()
    {
        int l = LengthWithoutExtras();
        int[] numbers = new int[l];
        char[] s1 = SNumbers;
        for(int i = 0; i < l; i++)
        {
            numbers[i] = (int)Char.GetNumericValue(s1[i]);
        }
        return numbers;
    }

    public void printNumbers()
    {
        Console.Write("\nprintNumbers: ");
        int l = LengthWithoutExtras();
        for (int i = 0; i < l; i++)
        {
            Console.Write("{0}, ",NumberFill[i]);
        }
    }

    public int[,] fillMatrix()
    {
        int[,] matrix = new int[row,col];
        if(ifOneRow())
        {
            int l = LengthWithoutExtras();
            int[] nums = NumberFill;
            int count = 0;
            for (int k = 0; k < col; k++)
            {
               matrix[0, k] = nums[count];
               count++;
            }
            return matrix;
        }
        else if(ColIsEven())
        {
            int l = LengthWithoutExtras();
            int[] nums = NumberFill;
            int count = 0;
            for (int i = 0; i < row; i++)
            {
                for (int k = 0; k < col; k++)
                {
                    matrix[i, k] = nums[count];
                    count++;
                }
            }
            return matrix;
        }
        else
        {
            Console.WriteLine("ERROR: fillMatrix, NOT A VALID ARRAY");
            return matrix;
        }
    }

    public bool isVectorOrScalar()
    {
        if (myChar[0] != '[' && myChar[0] != ';' && myChar[0] != ']' && myChar[(myChar.Length - 1)] != ']')
        {
            return true;
        }
        else
            return false;
    }
    public void printMatrix()
    {
        Console.WriteLine("\nPrintMatrix: ");
        for (int i = 0; i < row; i++)
        {
            for (int k = 0; k < col; k++)
            {
                Console.Write("\t{0}", FinalMatrix[i, k]);
            }
            Console.WriteLine();
        }
    }

    public bool ColIsEven()
    {
      
        int temp = 0;
        int l = myChar.Length; 
        int[] count = new int[row];
        int k = 0;

        bool ans = false;
        if (ProperStart())
        {
            for(int i = 1; i <l; i++)
            {
                if(myChar[i]!= ';' && myChar[i] != ']')
                {
                    if(myChar[i]!=' ')
                    {
                        temp++;
                    }
                }
                if(myChar[i]== ';' || myChar[i] == ']')
                {
                    count[k] = temp;
                    k++;
                    temp = 0;
                }
            }
        }
        for(int z = 0; z <row-1; z++)
        {
            if (count[z] == count[z + 1])
            {
                ans = true;
            }
            else
                return false;
        }
        return ans;

    }

    public bool ProperStart()
    {
        if(myChar[0] == '[')
        {
            return true;
        }
        else
        {
            Console.WriteLine("ERROR: ProperStart, Improper array format");
            return false;
        }
    }

    public bool isSquare()
    {
        if (row == col)
            return true;
        else
            return false;
    }
    public int[,] detArray()
    {
        int c = col-1;
        int r = row;
        int[,] array = new int[r, c];

        for(int i = 0; i < c; i++)
        {
            for (int k = 0; k < r; k++ )
            {
                array[k, i] = FinalMatrix[k, i];
            }
        }
        return array;
    }
/*
    public int[,] newDetArray()
    {
        int r = row;
        int c = col + (col - 1);
        int[,] array = detArray();
        int[,] newArray = new int[r, c];

        for(int i = 0; i<r; i++)
        {
            for(int k = 0; k < c; k++)
            {
                if( k < col)
                {
                    newArray[i, k] = FinalMatrix[i, k];
                }
                else
                {
                    newArray[r, c] = array[i, k];
                }
                
            }
        }
        return newArray;
    }

    public void printDet()
    {
        int c = col + (col - 1);
        int[,] array = newDetArray();
        for (int i = 0; i < row; i++)
        {
            for (int k = 0; k < c; k++)
            {
                Console.Write("\t{0}", array[i, k]);
            }
        }
    }
    */

    public bool ifOneRow()
    {
        if (row == 1)
        {
            return true;
        }
        else
            return false;
    }


    public void MatLab()
    {
        if (ProperStart())
        {
            if(seperate != ';')
            {
                Console.WriteLine("ERROR: IMPROPER FORMAT");
            }
            else if(ifOneRow())
            {
                Console.WriteLine("One Row Matrix: ");
                for(int i = 0; i < col; i++)
                {
                    Console.Write("\t{0}",FinalMatrix[0,i]);
                }
                Console.WriteLine();
                return;
            }
            else  if (ColIsEven())
            {
                printMatrix();
                //   myMatrix.printDet();
            }
            else
            {
                Console.WriteLine("ERROR: Not a vild array.");
            }
        }
        else
        {
            Console.WriteLine("ERROR: Not a valid array.");
        }
    }
}
