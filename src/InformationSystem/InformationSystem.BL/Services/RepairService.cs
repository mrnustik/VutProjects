using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InformationSystem.BL.Models.Car;
using InformationSystem.BL.Models.Material;
using InformationSystem.BL.Models.Repair;
using InformationSystem.BL.Models.User;
using InformationSystem.DAL;
using InformationSystem.DAL.Entities;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.ValueGeneration.Internal;

namespace InformationSystem.BL.Services
{
    public class MaterialService
    {
        private readonly ServiceDbContext _dbContext;
        public MaterialService(ServiceDbContext dbContext)
        {
            _dbContext = dbContext;
        }

        public IQueryable<MaterialModel> GetAllMaterials()
        {
            return _dbContext.Materials.Select(m => new MaterialModel
            {
                Id = m.Id,
                Name = m.Name,
                Price = m.Price
            }).AsQueryable();
        }

        public async Task RemoveMaterial(MaterialModel material)
        {
            var entity = await  _dbContext.Materials.FirstAsync(m => m.Id == material.Id);
            _dbContext.Materials.Remove(entity);
            await _dbContext.SaveChangesAsync();
        }

        public IQueryable<UsedMaterialModel> GetUsedMaterialsForRepair(Guid repairId)
        {
            return _dbContext.Repairs
                .First(r => r.Id == repairId)
                .UsedMaterials
                .Select(um => new UsedMaterialModel
                        {
                            Id = um.Id,
                            Ammount = um.Ammount,
                            Material = MaterialEntityToMaterialModel(_dbContext.Materials.FirstOrDefault(m => m.Id == um.MaterialId))
                        })
                        .AsQueryable();

        }

        private MaterialModel MaterialEntityToMaterialModel(MaterialEntity entity)
        {
            return new MaterialModel
            {
                Id = entity.Id,
                Name = entity.Name,
                Price = entity.Price
            };
        }

        public async Task AddOrUpdateMaterialToRepair(Guid repairId, UsedMaterialModel usedMaterial)
        {
            var entity = await _dbContext.Repairs
                .Include(r => r.UsedMaterials)
                .FirstAsync(r => r.Id == repairId);
            var material = entity.UsedMaterials.FirstOrDefault(m => m.Id == usedMaterial.Id);
            if (material == null)
            {
                material = new UsedMaterialEntity
                {
                    Id = usedMaterial.Id,
                    Ammount = usedMaterial.Ammount,
                    Material = _dbContext.Materials.FirstOrDefault(m => m.Id == usedMaterial.Material.Id),
                    Repair = entity
                };
                entity.UsedMaterials.Add(material);
                await _dbContext.SaveChangesAsync();
            }
            else
            {
                entity.UsedMaterials.Remove(material);
                material.Ammount = usedMaterial.Ammount;
                material.Material = _dbContext.Materials.FirstOrDefault(m => m.Id == usedMaterial.Material.Id);
                material.Repair = entity;
                entity.UsedMaterials.Add(material);
                await _dbContext.SaveChangesAsync();
            }
        }

        public async Task RemoveMaterialFromRepair(Guid repairId, UsedMaterialModel usedMaterial)
        {
            var entity = await _dbContext.Repairs
                .Include(r => r.UsedMaterials)
                .FirstAsync(r => r.Id == repairId);
            var material = entity.UsedMaterials.FirstOrDefault(m => m.Id == usedMaterial.Id);
            if (material != null)
            {
                entity.UsedMaterials.Remove(material);
                await _dbContext.SaveChangesAsync();
            }
        }

        public async Task AddOrUpdateMaterial(MaterialModel material)
        {
            var entity = await _dbContext.Materials.FirstOrDefaultAsync(m => m.Id == material.Id);
            if (entity == null)
            {
                entity = new MaterialEntity
                {
                    Id = material.Id,
                    Name = material.Name,
                    Price = material.Price
                };
                await _dbContext.AddAsync(entity);
            }
            else
            {
                entity.Name = material.Name;
                entity.Price = material.Price;
                _dbContext.Materials.Update(entity);
            }
            await _dbContext.SaveChangesAsync();
        }
    }
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
            var mechanics = (await _userManager.GetUsersInRoleAsync("mechanic"))
                .ToList();

            var repairs = _dbContext.Repairs
                .Where(r => r.ReservationTime == when)
                .ToList();
            return mechanics
                .Where(m => repairs.All(r => r.MechanicUserName != m.UserName))
                .Select(m => new UserModel{ Email = m.Email, Name = m.Name, Role = "mechanic"});
        }

        public IQueryable<RepairListModel> GetAllRepairsForMechanic(string userName)
        {
            return _dbContext.Repairs
                .Where(r => r.ReservationTime > DateTime.Today)
                .Where(r => r.MechanicUserName == userName)
                .Include(r => r.Car)
                .Select(repair => new RepairListModel
                {
                    Id = repair.Id,
                    Done = repair.Done,
                    CarName = repair.Car.Name,
                    CarManufacturer = repair.Car.CompanyName,
                    CarType = repair.Car.TypeName,
                    RepairType = repair.RepairType,
                    ReservationDate = repair.ReservationTime
                })
                .AsQueryable();
        }

        public async Task<RepairDetailModel> GetRepairDetailAsync(Guid repairId)
        {
            var repairEntity = await _dbContext.Repairs
                .Include(r => r.UsedMaterials)
                .Include(r => r.Car)
                .FirstAsync(r => r.Id == repairId);
            return new RepairDetailModel
            {
                Id = repairEntity.Id,
                RepairType = repairEntity.RepairType,
                Description = repairEntity.Description,
                Done = repairEntity.Done,
                ReservationDate = repairEntity.ReservationTime,
                Car = new CarDetailModel
                {
                    Id = repairEntity.Car.Id,
                    CarIdenitification = repairEntity.Car.CarIdentification,
                    CompanyName = repairEntity.Car.CompanyName,
                    TypeName = repairEntity.Car.TypeName,
                    Name = repairEntity.Car.Name
                },
                UsedMaterials = repairEntity.UsedMaterials.Select(
                    um => new UsedMaterialModel
                    {
                        Id = um.Id,
                        Ammount = um.Ammount,
                        Material = new MaterialModel
                        {
                            Id = um.Material.Id,
                            Name = um.Material.Name,
                            Price = um.Material.Price
                        }
                    })
            };
        }

        public async Task UpdateRepair(RepairDetailModel repair)
        {
            var repairEntity = await _dbContext.Repairs.FirstOrDefaultAsync(r => r.Id == repair.Id);
            repairEntity.Done = repair.Done;
            repairEntity.Description = repair.Description;
            _dbContext.Repairs.Update(repairEntity);
            await _dbContext.SaveChangesAsync();
        }
    }
}
