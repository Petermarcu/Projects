using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using Microsoft.AspNetCore.Mvc.ModelBinding;

namespace Chores.Models
{
    public class Chore
    {
        public string Name { get; set; }

        public Chore(string name)
        {
            this.Name = name;
        }
    }
}
