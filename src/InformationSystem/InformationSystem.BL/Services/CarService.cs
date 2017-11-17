using System;
using System.Linq;
using System.Threading.Tasks;
using InformationSystem.BL.Models.Car;
using InformationSystem.DAL;
using InformationSystem.DAL.Entities;

namespace InformationSystem.BL.Services
{
    public class CarService
    {
        private readonly ServiceDbContext dbContext;

        public CarService(ServiceDbContext dbContext)
        {
            this.dbContext = dbContext;
        }

        public IQueryable<CarListModel> GetAllCars()
        {
            return dbContext.Cars
                            .Select(CarEntityToListModel)
                            .AsQueryable();
        }

        public IQueryable<CarListModel> GetAllCarsByUser(string userId)
        {
            return dbContext.Cars
                            .Where(c => c.Owner.Id == userId)
                            .Select(CarEntityToListModel)
                            .AsQueryable();
        }

        public async Task DeleteCar(Guid carId)
        {
            dbContext.Cars.
            dbContext.Cars.RemoveAsync();
        }

        private CarListModel CarEntityToListModel(CarEntity entity){
            return new CarListModel
            {
                Id = entity.Id,
                Name = entity.Name,
                CarIdenitification = entity.CarIdentification
            };
        }
    }
}
