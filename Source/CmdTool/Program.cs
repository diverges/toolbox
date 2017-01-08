using System;
using System.ComponentModel;
using System.Linq;
using CmdTool;

namespace CmdTool
{
    [CmdTool("test")]
    [CmdDescription("This is a simple test class with some arguments.")]
    [CmdDescription("usage: TestCommand1 [<options] <directory>")]
    class TestCommand : ICommand
    {
        [Option("-v", "Verbose flag")]
        public bool Verbose = false;

        public void Run()
        {
            Console.WriteLine(Verbose);
        }
    }

    [CmdTool("test2")]
    [CmdDescription("hello weird c# features")]
    [CmdDescription("usage: TestCommand2 [<options] <directory>")]
    class TestCommand2 : ICommand
    {
        [Option("-v", "Verbose flag")]
        public bool Verbose = false;

        [Option("-r", "res")]
        public bool Retry = false;

        [Option("-n", "Sample number")]
        public int Number = 10;

        public void Run()
        {
            Console.WriteLine(Verbose);
            Console.WriteLine(Retry);
            Console.WriteLine(Number);
        }
    }

    [CmdTool("test3")]
    [CmdDescription("aaa")]
    [CmdDescription("uaaaaatory")]
    class TestCommand3 : ICommand
    {
        [Option("-n", "Sample number 1")]
        public int Number = 10;

        [Option("-r", "Sample number 2")]
        public int Apple = 10;

        public void Run()
        {
            Console.WriteLine(Apple);
        }
    }

    /// <summary>
    /// Collection of command line scripts.
    /// </summary>
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                ICommand app = Load.GetInstance(args);
                app.Run();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
    }
}
