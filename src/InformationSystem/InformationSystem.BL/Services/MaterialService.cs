using System;
using System.Linq;
using System.Threading.Tasks;
using InformationSystem.BL.Models.Material;
using InformationSystem.DAL;
using InformationSystem.DAL.Entities;
using Microsoft.EntityFrameworkCore;

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
}