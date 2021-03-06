﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace CmdTool
{

    interface ICommand
    {
        void Run();
    }

    class ErrorCommand : ICommand
    {
        private readonly string _m;

        public ErrorCommand(string message)
        {
            _m = message;
        }

        public void Run()
        {
            Console.WriteLine(_m);
        }
    }

    internal static class Load
    {
        /// <summary>
        /// Field Metadata
        /// </summary>
        private struct FieldDesc
        {
            // Public class member metadata, 
            // provides type information.
            public FieldInfo Field;

            // Store data from Option attribute,
            // we will use this to auto generate,
            // documentation for the class.
            public Option Option;

            public FieldDesc(FieldInfo field, Option option)
            {
                Field = field;
                Option = option;
            }
        }

        /// <summary>
        /// CmdTool Metadata
        /// </summary>
        private struct CmdDesc
        {
            // Class type, used for instantiation.
            public Type CmdType;

            // Collection of all public class members,
            // anything else is encapsulated,
            public Dictionary<string, FieldDesc> Fields;

            public CmdDesc(Type type, Dictionary<string, FieldDesc> fields)
            {
                CmdType = type;
                Fields = fields;
            }
        }

        // Available Command Line Tools in assembly.
        private static readonly Dictionary<string, CmdDesc> CommandDictionary = GetCmdTools();

        public static ICommand GetInstance (string[] args)
        {
            if (args.Length == 0)
            {
                return new ErrorCommand(Load.GetHelp());
            }

            if (args[0].ToLower().Equals("help"))
            {
                return new ErrorCommand((args.Length > 1) ? Load.GetHelp(args[1]) : Load.GetHelp());
            }

            // Search collection for first command.
            var cmdString = args.First();
            CmdDesc command = CommandDictionary[cmdString];

            // Instantiate
            dynamic val = Activator.CreateInstance(command.CmdType);

            // Fields should have a default value,
            // for any additional arguments we check
            // and set corresponding field.
            for (var i = 1; i < args.Length; ++i)
            {
                if (!args[i].StartsWith("-"))
                    continue;

                if (!command.Fields.ContainsKey(args[i]))
                {
                    return new ErrorCommand($"Invalid input parameter: {args[i]}\n" + Load.GetHelp(cmdString));
                }

                FieldInfo field = command.Fields[args[i]].Field;

                if (field.FieldType == typeof(bool))
                {
                    field.SetValue(val, true);
                }
                else
                {
                    field.SetValue(val, Convert.ChangeType(args[i+1], field.FieldType));
                }
            }
            return (ICommand) val;
        }

        public static string GetHelp()
        {
            // Usage information.
            var ret = "usage: miso <command> [<args>]\n\n" +
                         "Available commands:\n";

            // Print command metadata, as described by CmdDescription attribute.
            foreach (KeyValuePair<string, CmdDesc> elem in CommandDictionary)
            {
                IEnumerable<CmdDescription> description =
                    elem.Value.CmdType.GetCustomAttributes<CmdDescription>(false);
                ret += $"    {elem.Key}\t{description.First().Description}\n";
            }
            ret += "\nUse 'miso help <command>' for additional information.\n";
            return ret;
        }

        public static string GetHelp(string type)
        {
            // Command description,
            // we can append multiple CmdDescription attributes.
            CmdDesc command = CommandDictionary[type];
            IEnumerable<CmdDescription> usage =
                command.CmdType.GetCustomAttributes<CmdDescription>(false);
            var ret = usage.Aggregate("", (current, elem) => current + $"{elem.Description}\n");

            // Command input metadata
            return command.Fields.Aggregate(
                ret + "\n",
                (current, elem) => current + $"    {elem.Value.Option.Command}\t\t{elem.Value.Option.Description}\n");
        }

        private static Dictionary<string, CmdDesc> GetCmdTools()
        {
            return GetTypesWith<CmdTool>(true)
                .ToDictionary(
                    type => ((CmdTool) Attribute.GetCustomAttribute(type, typeof(CmdTool))).Command,
                    type =>
                    {
                        // Get all fields in current Type with Option attribute,
                        // store field metadata.
                        IEnumerable<FieldDesc> fields =
                            from fld in type.GetFields()
                            from opt in fld.GetCustomAttributes<Option>()
                            select new FieldDesc(fld, opt);
                        return new CmdDesc(type, fields.ToDictionary(f => f.Option.Command, f => f));
                    },
                    StringComparer.OrdinalIgnoreCase);
        }

        private static IEnumerable<Type> GetTypesWith<TAttribute>(bool inherit)
        {
            // Scan assembly for any Type with TAttribute
            return
                from assembly in AppDomain.CurrentDomain.GetAssemblies()
                from types in assembly.GetTypes()
                where types.IsDefined(typeof(TAttribute), inherit)
                select types;
        }
    }
}
