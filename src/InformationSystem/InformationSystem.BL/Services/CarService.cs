using System;
using System.Linq;
using System.Threading.Tasks;
using InformationSystem.BL.Models.Car;
using InformationSystem.DAL;
using InformationSystem.DAL.Entities;
using Microsoft.EntityFrameworkCore;

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

        public IQueryable<CarListModel> GetAllCarsByUser(string userEmail)
        {
            return dbContext.Cars
                .Where(c => c.OwnerEmail == userEmail)
                            .Select(CarEntityToListModel)
                            .AsQueryable();
        }

        public async Task DeleteCar(Guid carId)
        {
            var carEntity = await dbContext.Cars.FirstAsync(c => c.Id == carId);
            dbContext.Cars.Remove(carEntity);
            await dbContext.SaveChangesAsync();
        }

        public async Task CreateCar(CarDetailModel carModel, string userEmail)
        {
            var entity = CarDetailToCarEntity(carModel);
            entity.Id = new Guid();
            entity.OwnerEmail = userEmail;
            dbContext.Cars.Add(entity);
            await dbContext.SaveChangesAsync();
        }

        private CarEntity CarDetailToCarEntity(CarDetailModel carModel)
        {
            return new CarEntity
            {
                CarIdentification = carModel.CarIdenitification,
                CompanyName = carModel.CompanyName,
                Id = carModel.Id,
                Name = carModel.Name,
                TypeName = carModel.TypeName
            };
        }

        private CarListModel CarEntityToListModel(CarEntity entity){
            return new CarListModel
            {
                Id = entity.Id,
                Name = entity.Name,
                CarIdenitification = entity.CarIdentification
            };
        }

        private CarDetailModel CarEntityToDetailModel(CarEntity entity)
        {
            return new CarDetailModel()
            {
                Id = entity.Id,
                Name = entity.Name,
                CarIdenitification = entity.CarIdentification,
                CompanyName = entity.CompanyName,
                TypeName = entity.TypeName,
                OwnerEmail = entity.OwnerEmail
            };
        }

        public async Task<CarDetailModel> GetCarById(Guid carId)
        {
            return CarEntityToDetailModel(await dbContext.Cars.FirstAsync(c => c.Id == carId));
        }

        public async Task SaveCarAsync(CarDetailModel car)
        {
            var entity = await dbContext.Cars.FirstAsync(c => c.Id == car.Id);
            entity.CompanyName = car.CompanyName;
            entity.CarIdentification = car.CarIdenitification;
            entity.Name = car.Name;
            entity.TypeName = car.TypeName;
            dbContext.Cars.Update(entity);
            await dbContext.SaveChangesAsync();
        }
    }
}
