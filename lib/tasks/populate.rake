#lib/tasks/populate.rake
namespace :db do
  desc "Erase and fill database"
  task :populate => :environment do
    require 'populator'
    require 'faker'
    
    [College, Room, RoomProfile, User, Profile, Host].each(&:delete_all)
    
    College.populate 20 do |college|
      college.name = Populator.words(1..3).titleize
      college.street_address_1  = Faker::Address.street_address
      college.city              = Faker::Address.city
      college.state             = Faker::Address.us_state_abbr
    end
    

    
    User.populate 40 do |user|
      user.first_name   = Populator.words(1).titleize
      user.last_name    = Populator.words(1).titleize
      user.gender       = "Male"
      
      Host.populate 1 do |host|
        host.user_id    = user.id
        Room.populate 1 do |room|
          room.college_id       = 1 + rand(20)
          room.host_id          = host.id
          room.street_address_1 = Faker::Address.street_address
          room.street_address_2 = ""
          room.city             = Faker::Address.city
          room.state            = Faker::Address.us_state_abbr
          room.active           = true
          room.zip              = Faker::Address.zip_code
          
          RoomProfile.populate 1 do |room_profile|
            room_profile.room_id    = room.id
            room_profile.size_classification  = Populator.words(1..3)
            room_profile.bed_type             = Populator.words(1..3)
            room_profile.max_capacity         = rand(6) + 1
            room_profile.pets                 = Populator.words(1..3)
            room_profile.food                 = Populator.words(1..3)
            room_profile.roommates             = rand(4)
            room_profile.notes                = Populator.words(1..3)
            room_profile.cost                 = rand(10)+1
            
          end
        end
      end
    end
    
  end
end

