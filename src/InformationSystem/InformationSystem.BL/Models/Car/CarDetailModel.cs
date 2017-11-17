using System;
namespace InformationSystem.BL.Models.Car
{
    public class CarDetailModel
    {
        public Guid Id
        {
            get;
            set;
        }

        public string Name
        {
            get;
            set;
        }

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

        public string CompanyName
        {
            get;
            set;
        }

        public string TypeName
        {
            get;
            set;
        }
    }
}
