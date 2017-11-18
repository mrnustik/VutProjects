using System;
using System.ComponentModel.DataAnnotations;

namespace InformationSystem.BL.Models.Car
{
    public class CarDetailModel
    {
        public Guid Id
        {
            get;
            set;
        }

        [Required]
        public string Name
        {
            get;
            set;
        }

        [Required]
        public string CarIdenitification
        {
            get;
            set;
        }

        public string OwnerEmail
        {
            get;
            set;
        }

        [Required]
        public string CompanyName
        {
            get;
            set;
        }

        [Required]
        public string TypeName
        {
            get;
            set;
        }
    }
}
