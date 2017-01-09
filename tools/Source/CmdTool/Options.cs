using System;
using System.ComponentModel;

namespace CmdTool
{
    /// <summary>
    /// Registers a class which can be called from the command line. 
    /// Public class members represent flag options. Must also provide usage,
    /// option, and description attribute.
    /// Ex.
    ///
    /// [CmdTool("...")]
    /// [Description("...")]
    /// [Description("..."]
    /// class Test
    /// {
    ///     ...
    ///     [Option("...")]
    ///     public bool Flag = false;
    ///     ...
    /// }
    ///
    /// </summary>
    [AttributeUsage(AttributeTargets.Class, AllowMultiple = false)]
    public class CmdTool : Attribute
    {
        public string Command { get; private set; }

        public CmdTool(string command)
        {
            Command = command;
        }
    }

    [AttributeUsage(AttributeTargets.Class, AllowMultiple = true)]
    public class CmdDescription : Attribute
    {
        public string Description { get; private set; }

        public CmdDescription(string description)
        {
            Description = description;
        }
    }

    [AttributeUsage(AttributeTargets.Field, AllowMultiple = false)]
    public class Option : Attribute
    {
        public string Command { get; private set; }
        public string Description { get; private set; }

        public Option(string command, string description)
        {
            Command = command;
            Description = description;
        }
    }
}