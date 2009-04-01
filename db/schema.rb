# This file is auto-generated from the current state of the database. Instead of editing this file, 
# please use the migrations feature of Active Record to incrementally modify your database, and
# then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your database schema. If you need
# to create the application database on another system, you should be using db:schema:load, not running
# all the migrations from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended to check this file into your version control system.

ActiveRecord::Schema.define(:version => 20090401180629) do

  create_table "acts_as_xapian_jobs", :force => true do |t|
    t.string  "model",    :null => false
    t.integer "model_id", :null => false
    t.string  "action",   :null => false
  end

  add_index "acts_as_xapian_jobs", ["model", "model_id"], :name => "index_acts_as_xapian_jobs_on_model_and_model_id", :unique => true

  create_table "available_days", :force => true do |t|
    t.string   "day",        :limit => 50
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "colleges", :force => true do |t|
    t.string   "name"
    t.string   "street_address_1"
    t.string   "street_address_2"
    t.string   "city",             :limit => 50
    t.string   "state",            :limit => 2
    t.string   "zip",              :limit => 12
    t.string   "policy"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "favorites", :force => true do |t|
    t.integer  "renter_id",  :null => false
    t.integer  "room_id",    :null => false
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "homes", :force => true do |t|
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "hosts", :force => true do |t|
    t.integer  "user_id",    :null => false
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "levels", :force => true do |t|
    t.integer  "rank"
    t.string   "cleanliness", :limit => 50
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "modifications", :force => true do |t|
    t.integer  "transaction_id",                    :null => false
    t.boolean  "approved",       :default => false
    t.datetime "created_at"
    t.date     "check_in"
    t.date     "check_out"
    t.integer  "guests"
    t.string   "comments"
    t.datetime "updated_at"
  end

  create_table "profiles", :force => true do |t|
    t.integer  "user_id",                                             :null => false
    t.integer  "college_id",                                          :null => false
    t.string   "image"
    t.date     "birthdate"
    t.date     "grad_year"
    t.string   "phone",              :limit => 20
    t.string   "living_style"
    t.string   "bedtime"
    t.string   "interests"
    t.boolean  "smoker",                           :default => false
    t.datetime "created_at"
    t.datetime "updated_at"
    t.string   "photo_file_name"
    t.string   "photo_content_type"
    t.integer  "photo_file_size"
    t.datetime "photo_updated_at"
  end

  create_table "ratings", :force => true do |t|
    t.integer  "rating",                      :default => 0
    t.datetime "created_at",                                  :null => false
    t.string   "rateable_type", :limit => 15, :default => "", :null => false
    t.integer  "rateable_id",                 :default => 0,  :null => false
    t.integer  "user_id",                     :default => 0
  end

  add_index "ratings", ["user_id"], :name => "fk_ratings_user"

  create_table "renters", :force => true do |t|
    t.integer  "user_id",    :null => false
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "room_images", :force => true do |t|
    t.integer  "room_profile_id", :null => false
    t.string   "image"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "room_profile_days", :force => true do |t|
    t.integer  "day_id",          :null => false
    t.integer  "room_profile_id", :null => false
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "room_profiles", :force => true do |t|
    t.integer  "room_id",                                                                             :null => false
    t.integer  "level_id"
    t.string   "size_classification", :limit => 50
    t.string   "bed_type",            :limit => 50
    t.integer  "max_capacity"
    t.string   "pets",                :limit => 50
    t.string   "food",                :limit => 100
    t.integer  "roommates"
    t.boolean  "internet",                                                         :default => false
    t.boolean  "on_campus",                                                        :default => false
    t.string   "notes"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.decimal  "cost",                               :precision => 5, :scale => 2
  end

  create_table "room_reviews", :force => true do |t|
    t.integer  "rating"
    t.string   "review"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.integer  "transaction_id"
  end

  create_table "rooms", :force => true do |t|
    t.integer  "host_id",                                           :null => false
    t.integer  "college_id",                                        :null => false
    t.string   "street_address_1"
    t.string   "street_address_2"
    t.string   "city",             :limit => 50
    t.string   "state",            :limit => 2
    t.string   "zip",              :limit => 12
    t.boolean  "active",                         :default => false
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "transactions", :force => true do |t|
    t.integer  "renter_id",                     :null => false
    t.datetime "created_at"
    t.boolean  "active",     :default => false
    t.datetime "updated_at"
    t.integer  "room_id"
    t.boolean  "approved"
  end

  create_table "user_reviews", :force => true do |t|
    t.integer  "profile_id",                    :null => false
    t.integer  "user_id",                       :null => false
    t.integer  "rating"
    t.string   "review"
    t.boolean  "status",     :default => false
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "users", :force => true do |t|
    t.string   "username",                  :limit => 40
    t.string   "first_name",                :limit => 100, :default => "",        :null => false
    t.string   "email",                     :limit => 100
    t.string   "crypted_password",          :limit => 40
    t.string   "salt",                      :limit => 40
    t.datetime "created_at"
    t.datetime "updated_at"
    t.string   "remember_token",            :limit => 40
    t.datetime "remember_token_expires_at"
    t.string   "last_name",                 :limit => 50
    t.integer  "profile_id"
    t.string   "activation_code",           :limit => 40
    t.datetime "activated_at"
    t.string   "state",                                    :default => "passive"
    t.datetime "deleted_at"
    t.string   "password_reset_code",       :limit => 40
    t.string   "user_type",                 :limit => 40
    t.boolean  "is_admin",                                 :default => false
  end

  add_index "users", ["username"], :name => "index_users_on_login", :unique => true

end
