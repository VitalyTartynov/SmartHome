﻿using System.Collections.Generic;
using Microsoft.AspNetCore.Mvc;
using ServerApi.Models;

namespace ServerApi.Controllers
{
    [Route("api/[controller]")]
    public class TemperatureController : Controller
    {
        private readonly IStorageRepository<Temperature> _temperaturesRepository;

        public TemperatureController(IStorageRepository<Temperature> temperaturesRepository)
        {
            _temperaturesRepository = temperaturesRepository;
        }

        // GET: api/values
        [HttpGet]
        public IEnumerable<Temperature> GetAll()
        {
            return _temperaturesRepository.GetAll();
        }

        // GET api/values/5
        [HttpGet("{id}")]
        public string Get(int id)
        {
            return "value";
        }

        // POST api/values
        [HttpPost]
        public void Post([FromBody]string value)
        {
        }

        // PUT api/values/5
        [HttpPut("{id}")]
        public void Put(int id, [FromBody]string value)
        {
        }

        // DELETE api/values/5
        [HttpDelete("{id}")]
        public void Delete(int id)
        {
        }
    }
}
