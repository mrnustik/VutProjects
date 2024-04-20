using Microsoft.EntityFrameworkCore.Migrations;
using System;
using System.Collections.Generic;

namespace InformationSystem.DAL.Migrations
{
    public partial class UpdatedCarEntityMigration : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Cars_AspNetUsers_OwnerId",
                table: "Cars");

            migrationBuilder.DropIndex(
                name: "IX_Cars_OwnerId",
                table: "Cars");

            migrationBuilder.DropColumn(
                name: "OwnerId",
                table: "Cars");

            migrationBuilder.AddColumn<string>(
                name: "CompanyName",
                table: "Cars",
                type: "nvarchar(max)",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "Name",
                table: "Cars",
                type: "nvarchar(max)",
                nullable: false,
                defaultValue: "");

            migrationBuilder.AddColumn<string>(
                name: "OwnerEmail",
                table: "Cars",
                type: "nvarchar(max)",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "TypeName",
                table: "Cars",
                type: "nvarchar(max)",
                nullable: true);
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "CompanyName",
                table: "Cars");

            migrationBuilder.DropColumn(
                name: "Name",
                table: "Cars");

            migrationBuilder.DropColumn(
                name: "OwnerEmail",
                table: "Cars");

            migrationBuilder.DropColumn(
                name: "TypeName",
                table: "Cars");

            migrationBuilder.AddColumn<string>(
                name: "OwnerId",
                table: "Cars",
                nullable: true);

            migrationBuilder.CreateIndex(
                name: "IX_Cars_OwnerId",
                table: "Cars",
                column: "OwnerId");

            migrationBuilder.AddForeignKey(
                name: "FK_Cars_AspNetUsers_OwnerId",
                table: "Cars",
                column: "OwnerId",
                principalTable: "AspNetUsers",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);
        }
    }
}
