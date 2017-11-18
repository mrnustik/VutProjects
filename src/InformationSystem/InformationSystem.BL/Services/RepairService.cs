using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InformationSystem.BL.Models.Car;
using InformationSystem.BL.Models.Repair;
using InformationSystem.BL.Models.User;
using InformationSystem.DAL;
using InformationSystem.DAL.Entities;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;

namespace InformationSystem.BL.Services
{
    public class RepairService
    {
        private readonly ServiceDbContext _dbContext;
        private readonly UserManager<ApplicationUser> _userManager;
        public RepairService(ServiceDbContext dbContext, UserManager<ApplicationUser> userManager)
        {
            _dbContext = dbContext;
            _userManager = userManager;
        }

        public async Task CreateRepairOrder(RepairDetailModel repairDetail)
        {
            var entity = await RepairDetailToEntity(repairDetail);
            _dbContext.Repairs.Add(entity);
            await _dbContext.SaveChangesAsync();
        }

        public IQueryable<RepairListModel> GetAllRepairsByUser(string userName)
        {
            return _dbContext.Cars
                .Where(c => c.OwnerEmail == userName)
                .Join(_dbContext.Repairs, entity => entity.Id, entity => entity.Car.Id, (car, repair) =>
                    new RepairListModel
                    {
                        Id = repair.Id,
                        Done = repair.Done,
                        CarName = car.Name,
                        RepairType = repair.RepairType,
                        ReservationDate = repair.ReservationTime,
                        MechanicUserName = repair.MechanicUserName,
                        MechanicName = _dbContext.Users.FirstOrDefault(u => u.UserName == repair.MechanicUserName).Name
                    }).AsQueryable();

        }

        private async Task<RepairEntity> RepairDetailToEntity(RepairDetailModel repairDetail)
        {
            var carEntity = await _dbContext.Cars.FirstAsync(c => c.Id == repairDetail.Car.Id);
            return new RepairEntity
            {
                Car = carEntity,
                Description = repairDetail.Description,
                Done = false,
                Duration = 1,
                MechanicUserName = repairDetail.Mechanic.Email,
                RepairType = repairDetail.RepairType,
                ReservationTime = repairDetail.ReservationDate
            };
        }

        public async Task<IEnumerable<UserModel>> FindAvailableMechanics(DateTime when)
        {
            var mechanics = await _userManager.GetUsersInRoleAsync("mechanic");

            var repairs = _dbContext.Repairs.Where(r => r.ReservationTime == when);
            var mechanicsList = mechanics
                .ToList();
            mechanicsList
                .RemoveAll(m => repairs.Any(a => a.MechanicUserName == m.UserName));
            return mechanicsList
                .Select(m => new UserModel{ Email = m.Email, Name = m.Name, Role = "mechanic"});
        }
    }
}
