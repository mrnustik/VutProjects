using Microsoft.EntityFrameworkCore.Migrations;
using System;
using System.Collections.Generic;

namespace InformationSystem.DAL.Migrations
{
    public partial class AddNameToUser : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Repairs_AspNetUsers_MechanicId",
                table: "Repairs");

            migrationBuilder.DropIndex(
                name: "IX_Repairs_MechanicId",
                table: "Repairs");

            migrationBuilder.DropColumn(
                name: "MechanicId",
                table: "Repairs");

            migrationBuilder.AddColumn<string>(
                name: "MechanicUserName",
                table: "Repairs",
                type: "nvarchar(max)",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "Name",
                table: "AspNetUsers",
                type: "nvarchar(max)",
                nullable: true);
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "MechanicUserName",
                table: "Repairs");

            migrationBuilder.DropColumn(
                name: "Name",
                table: "AspNetUsers");

            migrationBuilder.AddColumn<string>(
                name: "MechanicId",
                table: "Repairs",
                nullable: true);

            migrationBuilder.CreateIndex(
                name: "IX_Repairs_MechanicId",
                table: "Repairs",
                column: "MechanicId");

            migrationBuilder.AddForeignKey(
                name: "FK_Repairs_AspNetUsers_MechanicId",
                table: "Repairs",
                column: "MechanicId",
                principalTable: "AspNetUsers",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);
        }
    }
}
