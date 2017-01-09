using System;
using System.ComponentModel;
using System.Linq;
using CmdTool;

namespace CmdTool
{

    [CmdTool("SimpleCommand")]
    [CmdDescription("Description of command.")]
    class SimpleCommand : ICommand
    {
        [Option("-v", "Allow input arguments too")]
        public bool Verbose = false;

        public void Run()
        {
            Console.WriteLine(Verbose);
        }
    }

    [CmdTool("DifferentCommand")]
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

    [CmdTool("ThirdCommand")]
    [CmdDescription("We cab support multiple CmdDescription attributes.")]
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

    [CmdTool("AnotherCommand")]
    [CmdDescription("This class takes integers as input!")]
    [CmdDescription("woah!")]
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
                // Any issues float to here...
                Console.WriteLine(e.Message);
            }
        }
    }
}
